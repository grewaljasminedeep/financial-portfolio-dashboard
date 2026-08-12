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
}