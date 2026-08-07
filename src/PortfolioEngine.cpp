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