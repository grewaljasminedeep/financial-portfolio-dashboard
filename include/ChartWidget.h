#pragma once

#include "DbConnection.h"
#include <QWidget>
#include <QString>

class ChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartWidget(DbConnection& db, QWidget* parent = nullptr);
    void setTicker(const QString& ticker);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    DbConnection& db_;
    QString ticker_;

    struct PricePoint {
        int dayIndex; // O..N
        double price;
    };
    std::vector<PricePoint> loadPrices(const QString& ticker);
};