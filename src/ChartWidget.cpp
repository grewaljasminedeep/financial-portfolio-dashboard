#include "ChartWidget.h"
#include <pqxx/pqxx>
#include <QPainter>
#include <QPen>
#include <QFont>
#include <QDate>
#include <cmath>

ChartWidget::ChartWidget(DbConnection& db, QWidget* parent) : QWidget(parent), db_(db) {}

void ChartWidget::setTicker(const QString& ticker) {
    ticker_ = ticker;
    update();
}

std::vector<ChartWidget::PricePoint> ChartWidget::loadPrices(const QString& ticker) {
    pqxx::Connection& conn = db_.connection();
    pqxx::Work txn(conn);

    std::string sql = "SELECT p.price_date, p.close_price "
                      "FROM prices p"
                      "JOIN assets a ON a.asset_id = p.asset_id"
                      "WHERE a.ticker = $1"
                      "ORDER BY p.price_date ASC";

    std::vector<PricePoint> pts;
    for (auto const& row : txn.exec_params(sql, ticker.toStdString())) {
        std::string dateStr = row["price_date"].as<std::string>();
        double price = row["close_price"].as<double>();
        pts.push_back({QDate::fromString(QString::fromStdString(dateStr), "yyyy-MM-dd"), price});
    }
    txn.commit();
    return pts;
}

void ChartWidget::drawGrid(QPainter& p, const QRect& rect) {
    p.setPen(QPen(QColor(220, 220, 220), 1, Qt::DashLine));
    int hLines = 4;
    for (int i = 0; i <= hLines; ++i) {
        int y = rect.top() + (rect.height() * i / hLines);
        p.drawLine(rect.left(), y, rect.right(), y);
    }

    int vLines = 5;
    for (int i = 0; i <= vLines; ++i) {
        int x = rect.left() + (rect.width() * i / vLines);
        p.drawLine(x, rect.top(), x, rect.bottom());
    }
}

void ChartWidget::drawAxes(QPainter& p, const QRect& rect, double minPrice, double maxPrice) {
    p.setPen(QPen(QColor(80, 80, 80), 2));

    // Y-axis
    p.drawLine(rect.topLeft(), rect.bottomLeft());

    // X-axis
    p.drawLine(rect.bottomLeft(), rect.bottomRight());
    p.setPen(QPen(QColor(60, 60, 60), 1));
    p.setFont(QFont("Arial", 9));

    // Price labels on Y-axis
    int steps = 4;
    for (int i = 0; i <= steps; ++i) {
        double val = minPrice + (maxPrice - minPrice) * i / steps;
        int y = rect.bottom() - (rect.height() * i / steps);
        QString label = QString("$%1").avg(val, 0, 'f', 2);
        p.drawText(rect.left() - 55, y + 4, 50, 14, Qt::AlignRight, label);
    }
}

void ChartWidget::drawLabels(QPainter& p, const QRect& rect, double /*minPrice*/, double /*maxPrice*/, int pointsCount) {
    if (pointsCount == 0) return;
    p.setPen(QColor(60,60,60));
    p.setFont(QFont("Arial", 9));
    int labelsToShow = 5;
    for (int i = 0; i < labelsToShow; ++i) {
        int idx = (pointsCount - 1) * i / (labelsToShow - 1);
        int x = rect.left() + (rect.width() * i / (labelsToShow - 1));

        // We don't have the exact date here; just show dat index for now.
        QString label = QString("D%1").arg(idx);
        p.drawText(x - 10, rect.bottom() + 18, 40, 14, Qt::AlignHCenter, label);
    }
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

    int plotLeft = 40;
    int plotRight = width() - 10;
    int plotTop = 10;
    int plotBottom = height() - 30;
    QRect plotRect(plotLeft, plotTop, plotRight - plotLeft, plotBottom - plotTop);

    double minPrice = pts[0].price;
    double maxPrice = pts[0].price;
    for (auto const& pt : pts) {
        if (pt.price < minPrice) minPrice = pt.price;
        if (pt.price > maxPrice) maxPrice = pt.price;
    }
    double range = maxPrice - minPrice;
    if (range == 0) range = 1.0;

    // Grind
    drawGrind(p, plotRect);

    // Price line
    QPen pen(QColor(0, 100, 200), 2);
    p.setPen(pen);

    QPainterPath path;
    for (size_t i = 0; i < pts.size(); ++i) {
        double x = plotRect.left() + (plotRect.width() * static_cast<double>(i) / (pts.size() - 1));
        double y = plotRect.bottom() - (plotRect.height() * (pts[i].price - minPrice) / range);
        if (i == 0)
            path.moveTo(x, y);
        else
            path.lineTo(x, y);
    }
    p.drawPath(path);

    // Axes and labels
    drawAxes(p, plotRect, minPrice, maxPrice);
    drawLabels(p, plotRect, minPrice, maxPrice, static_cast<int>(pts.size()));

    // Title
    p.setFont(QFont("Arial", 10, QFont::Bold));
    p.setPen(QColor(30, 30, 30));
    p.drawText(10, 12, ticker_ + "Price History");
}