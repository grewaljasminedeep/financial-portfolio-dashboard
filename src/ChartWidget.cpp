#include "ChartWidget.h"
#include <pqxx/pqxx>
#include <QPainter>
#include <QPen>
#include <QFont>

ChartWidget::ChartWidget(DbConnection& db, QWidget* parent) : QWidget(parent), db_(db) {}

void ChartWidget::setTicker(const QString& ticker) {
    ticker_ = ticker;
    update();
}

std::vector<ChartWidget::PricePoint> ChartWidget::loadPrices(const QString& ticker) {
    pqxx::Connection& conn = db_.connection();
    pqxx::Work txn(conn);

    std::string sql = "SELECT p.close_price"
                      "FROM prices p"
                      "JOIN assets a ON a.asset_id = p.asset_id"
                      "WHERE a.ticker = $1"
                      "ORDER BY p.price_date ASC";

    std::vector<PricePoint> pts;
    int i = 0;
    for (auto const& row : txn.exec_params(sql, ticker.toStdString())) {
        pts.push_back({i++, row["close_price"].as<double>()});
    }
    txn.commit();
    return pts;
}

void ChartWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);

p.setRenderHint(QPainter::Antialiasing, true);
    
    if (ticker_.isEmpty()) {
        p.drawText(rect(), Qt::AlignCenter, "Select an asset");
        return;
    }

    auto pts = loadPrices(ticker_);
    if (pts.empty()) {
        p.drawText(rect(), Qt::AlignCenter, "No price data");
        return;
    }

    int w = width() - 40;
    int h = height() - 40;
    int offsetX = 30;
    int offsetY = 20;

    double minPrice = pts[0].price;
    double maxPrice = pts[0].price;
    for (auto const& pt : pts) {
        if (pt.price < minPrice) minPrice = pt.price;
        if (pt.price > maxPrice) maxPrice = pt.price;
    }
    double range = maxPrice - minPrice;
    if (range == 0) range = 1.0;

    QPen pen(QColor(0, 100, 200), 2);
    p.setPen(pen);

    QPainterPath path;
    for (size_t i = 0; i < pts.size(); ++i) {
        double x = offsetX + (w * static_cast<double>(pts[i].dayIndex) / (pts.size() - 1));
    }
}