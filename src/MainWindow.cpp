#include "MainWindow.h"
#include "PortfolioEngine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QFont>
#include <QPalette>

MainWindow::MainWindow(const AppConfig& cfg, QWidget* parent) : QMainWindow(parent), cfg_(cfg), db_(cfg.database) {
    setWindowTitle("Financial Portfolio Dashboard");
    model_ = new PortfolioModel(this);
    table_ = new QTableView();
    table_->setModel(model_);
    table_->setAlternatingRowColors(true);
    chart_ = new ChartWidget(db_, this);
    chart_->setMinimumHeight(220);
    tickerInput_ = new QLineEdit();
    tickerInput_->setPlaceholderText("Enter ticker (e.g., AAPL)");
    connect(tickerInput_, &QLineEdit::returnPressed, this, [this]() {
        onTickerChanged(tickerInput_->text());
    });

    // Summary panel
    auto* summaryGroup = new QGroupBox("Portfolio Summary");
    auto* summaryLayout = new QHBoxLayout();
          QFont titleFont("Arial", 11, QFont::Bold);
          QFont valueFont("Arial", 12);
          totalValueLabel_ = new QLabel("Total Value: $0.00");
          totalValueLabel_->setFont(valueFont);
          totalCostLabel_ = new QLabel("Cost Basis: $0.00");
          totalCostLabel_->setFont(valueFont);
          totalPnLLabel_ = new QLabel("Unrealized P&L: $0.00");
          totalPnLLabel_->setFont(valueFont);
          summaryLayout->addWidget(totalValueLabel_);
          summaryLayout->addWidget(totalCostLabel_);
          summaryLayout->addWidget(totalPnLLabel_);
          summaryGroup->setLayout(summaryLayout);

    // Right side: chart + ticker input
    auto* rightLayout = new QVBoxLayout();
          rightLayout->addWidget(new QLabel("Price Chart"));
          rightLayout->addWidget(tickerInput_);
          rightLayout->addWidget(chart_);
    auto* rightWidget = new QWidget();
          rightWidget->setLayout(rightLayout);
    auto* splitter = new QSplitter(Qt::Horizontal);
          splitter->addWidget(table_);
          splitter->addWidget(rightWidget);
          splitter->setStretchFactor(0, 1);
          splitter->setStretchFactor(1, 1);
    auto* mainLayout = new QVBoxLayout();
          mainLayout->addWidget(summaryGroup);
          mainLayout->addWidget(splitter);
    auto* central = new QWidget();
          central->setLayout(mainLayout);
          setCentralWidget(central);

    //Market data worker
    worker_ = new MarketDataWorker(db_, cfg.watchlist, cfg.market_data.poll_interval_seconds, this);
    connect(worker_, &MarketDataWorker::newPricesAvailable, this, &MainWindow::refreshPortfolio);
    worker_->start();
    refreshPortfolio();
}

MainWindow::~MainWindow() {
    if (worker_) {
        worker_->stop();
        worker_->wait();
    }
}

void MainWindow::refreshPortfolio() {
    PortfolioEngine engine(db_);
    auto positions = engine.getPositions();
    auto summary = engine.getSummary(positions);
    model_->setPositions(positions);
    totalValueLabel_->setText(QString("Total Value: $%1").arg(summary.total_market_value, 0, 'f', 2));
    totalCostLabel_->setText(QString("Cost Basis: $%1").arg(summary.total_cost_basis, 0, 'f', 2));
    double pnl = summary.total_unrealized_pnl;
    QString pnlText = QString("Unrealized P&L: $%1").arg(pnl, 0, 'f', 2);
    QPalette pal = totalPnLLabel_->palette();
    pal.setColor(QPalette::WindowText, pnl >= 0 ? QColor(0, 120, 0) : QColor(180, 0, 0));
    totalPnLLabel_->setPalette(pal);
    totalPnLLabel_->setText(pnlText);
}

void MainWindow::onTickerChanged(const QString& ticker) {
    if (ticker.trimmed().isEmpty()) return;
    chart_->setTicker(ticker.toUpper());
}