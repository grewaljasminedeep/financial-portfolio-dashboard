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
}