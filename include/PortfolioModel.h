#pragma once

#include "PortfolioEngine.h"
#include <QAbstractTableModel>
#include <vector>

class PortfolioModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit PortfolioModel(QObject* parent = nullptr);
    void setPositions(const std::vector<AssetPosition>& positions);
    const std::vector<AssetPosition>& positions() const { return positions_; }
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount (const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
private:
    std::vector<AssetPosition> positions_;
};