#pragma once

#include "DbConnection.h"
#include "MarketDataWorker.h"
#include "PortfolioModel.h"
#include "ChartWidget.h"
#include <QMainWindow>
#include <QTableView>
#include <QLineEdit>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(const AppConfig& cfg, QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void refreshPortfolio();
    void onTickerChanged(const QString& ticker);

private:
    AppConfig cfg_;
    DbConnection db_;
    MarketDataWorker* worker_;
    PortfolioModel* model_;
    QTableView* table_;
    ChartWidget* chart_;
    QLineEdit* tickerInput_;

    QLabel* totalValueLabel_;
    QLabel* totalCostLabel_;
    QLabel* totalPnLLabel_;
};