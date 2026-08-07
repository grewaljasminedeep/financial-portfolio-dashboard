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