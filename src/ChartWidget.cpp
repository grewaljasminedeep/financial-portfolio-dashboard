#include "ChartWidget.h"
#include <pqxx/pqxx>
#include <QPainter>
#include <QPen>
#include <QFont>

ChartWidget::ChartWidget(DbConnection& db, QWidget* parent) : QWidget(parent), db_(db) {}

void ChartWidget::setTicker(const QString& ticker) {
    
}