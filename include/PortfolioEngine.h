#pragma once

#include "DbConnection.h"
#include <string>
#include <vector>

struct AssetPosition {
    int asset_id;
    std::string ticker;
    double quantity; //net shares
    double avg_cost; //average cost per share
    double current_price; //latest price
    double market_value; //quantity * current_price
    double cost_basis; //quantity * avg_cost
    double unrealized_pnl; //market_value - cost_basis
};

struct PortfolioSummary {
    double total_market_value;
    double total_cost_basis;
    double total_unrealized_pnl;
};