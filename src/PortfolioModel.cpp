#include "PortfolioModel.h"

PortfolioModel::PortfolioModel(QObject* parent) : QAbstractTableModel(parent) {}

void PortfolioModel::setPositions(const std::vector<AssetPosition>& positions) {
    positions_ = positions;
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
}

int PortfolioModel::rowCount(const QModelIndex&) const {
    
}