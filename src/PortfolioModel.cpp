#include "PortfolioModel.h"

PortfolioModel::PortfolioModel(QObject* parent) : QAbstractTableModel(parent) {}

void PortfolioModel::setPositions(const std::vector<AssetPosition>& positions) {
    positions_ = positions;
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

int PortfolioModel::rowCount(const QModelIndex&) const {
    return static_cast<int>(positions_.size());
}

int PortfolioModel::columnCount(const QModelIndex&) const {
    return 6; // Ticker, Qty, AvgCost, Price, MarketValue, PnL
}

QVariant PortfolioModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole) return {};
    auto const& p = positions_[static_cast<size_t>(index.row())];
    switch (index.column()) {
        case 0: return QString::fromStdString(p.ticker);
        case 1: return p.quantity;
        case 2: return p.avg_cost;
        case 3: return p.current_prices;
        case 4: return p.market_value;
        case 5: return p.unrealized_pnl;
        default: return {};
    }
}

QVariant PortfolioModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) return {};
    switch (section) {
        case 0: return "Ticker";
        case 1: return "Qty";
        case 2: return "Avg Cost";
        case 3: return "Price";
    }
}