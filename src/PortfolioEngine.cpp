#include "PortfolioEngine.h"
#include <pqxx/pqxx>
#include <unordered_map>
#include <cmath>

PortfolioEngine::PortfolioEngine(DbConnection& db)
    : db_(db) {}

std::vector<AssetPosition> PortfolioEngine::getPositions() {
    pqxx::work txn(conn);