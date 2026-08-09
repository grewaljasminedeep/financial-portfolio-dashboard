#include "PortfolioEngine.h"
#include <pqxx/pqxx>
#include <unordered_map>
#include <cmath>

PortfolioEngine::PortfolioEngine(DbConnection& db)
    : db_(db) {}

std::vector<AssetPosition> PortfolioEngine::getPositions() {
    pqxx::work txn(conn);

    // Build net quantity and cost basis per asset from transactions
    std::unordered_map<int, double> net_qty;
    std::unordered_map<int, double> total_cost;

    std::string txSql = "SELECT asset_id, transaction_type, quantity, price_per_unit"
                        " FROM transactions";

    for (auto const& row : txn.query(std::string(txSql))) {
        int asset_id = row["asset_id"].as<int>();
        std::string type = row["transaction_type"].as<std::string>();
        double qty = row["quantity"].as<double>();
        double price = row["price_per_unit"].as<double>();

        if (type == "BUY") {
            net_qty[asset_id] += qty;
            total_cost[asset_id] += qty * price;
        } else if (type == "SELL") {
            net_qty[asset_id] -= qty;
            // For simplicity, reduce cost basis proportionally
            double avg = (total_cost[asset_id] / (net_qty[asset_id] + qty));
            total_cost[asset_id] -= qty * avg;
        }
    }

    // Get latest prices
    std::unordered_map<int, double> latest_price;
    std::string priceSql = " SELECT p.asset_id, p.close_price"
                           " FROM prices p"
                           " JOIN (SELECT asset_id, MAX(price_date) AS max_date"
                           " FROM prices GROUP BY(asset_id) x"
                           " ON p.asset_id = x.asset_id AND p.price_date = x.max_date";

    for (auto const& row : txn.query(std::string(priceSql))) {
        int asset_id = row["asset_id"].as<int>();
        double price = row["close_price"].as<double>();
        latest_price[asset_id] = price;
    }

    // Get tickers
    std::unordered_map<int, std::string> ticker_map;
    std::string assetSql = "SELECT asset_id, ticker FROM assets";
    for (auto const& row : txn.query(std::string(assetSql))) {
        ticker_map[row["asset_id"].as<int>()] = row["ticker"].as<std::string>();
    }

    txn.commit();

    std::vector<AssetPosition> positions;
    for (auto const& [asset_id, qty] : net_qty) {
        if (qty <= 0) continue;
        double avg_cost = (qty > 0) ? (total_cost[asset_id] / qty) : 0.0;
        double cur_price = latest_price.count(asset_id) ? latest_price[asset_id] : 0.0;
        double mkt_val = qty * cur_price;
        double cost_basis = qty * avg_cost;
        double unrealized = mkt_val - cost_basis;

        positions.push_back({
            asset_id,
            ticker_map[asset_id],
            qty,
            avg_cost,
            cur_price,
            mkt_val,
            cost_basis,
            unrealized
        });
    }

    return positions;
}

PortfolioSummary PortfolioEngine::getSummary(const std::vector<AssetPosition>& positions) {
    PortfolioSummary s{};
    for (auto const& p : positions) {
        s.total_market_value += p.market_value;
        s.total_cost_basis += p.cost_basis;
        s.total_unrealized_pnl += p.unrealized_pnl;
    }
    return s;
}