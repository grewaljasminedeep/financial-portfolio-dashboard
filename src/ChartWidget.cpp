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
}