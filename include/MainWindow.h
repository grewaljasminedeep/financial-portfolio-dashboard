#pragma once

#include "DbConnection.h"
#include "MarketDataWorker.h"
#include "PortfolioModel.h"
#include "ChartWidget.h"
#include <QMainWindow>
#include <QTableView>
#include <QLineEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void refreshPortfolio();
    void onTickerChanged(const QString& ticker);

private:
    DbConnection db_;
    MarketDataWorker* worker_;
    PortfolioModel* model_;
    QTableView* table_;
    ChartWidget* chart_;
    QLineEdit* tickerInput_;
};