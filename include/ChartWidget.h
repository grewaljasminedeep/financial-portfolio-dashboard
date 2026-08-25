#pragma once

#include "DbConnection.h"
#include <QWidget>
#include <QString>
#include <vector>

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
        QDate date;
        double price;
    };
    std::vector<PricePoint> loadPrices(const QString& ticker);
    void drawGrid(QPainter& p, const QRect& rect);
    void drawAxes(QPainter& p, const QRect& rect, double minPrice, double maxPrice);
    void drawLabels(QPainter& p, const QRect& rect, double minPrice, double maxPrice, int pointsCount);
};