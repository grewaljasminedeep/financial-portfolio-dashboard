#include "MainWindow.h"
#include "PortfolioEngine.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), db_({ "localhost", 5432, "portfolio_db", "trader", "secure_pass" }) {
    setWindowTitle("Financial Portfolio Dashboard");
    model_ = new PortfolioModel(this);
    table_ = new QTableView();
    table_->setModel(model_);
    table_->setAlternatingRowColors(true);
    chart_ = new ChartWidget(db_, this);
    chart_->setMinimumHeight(200);
    tickerInput_ = new QLineEdit();
    tickerInput_->setPlaceholderText("Enter ticker (e.g., AAPL)");
    connect(tickerInput_, &QLineEdit::returnPressed, this, [this]() {
        onTickerChanged(tickerInput_->text());
    });

    auto* rightLayout = new QVBoxLayout();
    rightLayout->addWidget(new QLabel("Price Chart"));
    rightLayout->addWidget(tickerInput_);
    rightLayout->addWidget(chart_);
    auto* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(table_);
    auto* rightWidget = new QWidget();
    rightWidget->setLayout(rightLayout);
    splitter->addWidget(rightWidget);
    setCentralWidget(splitter);

    //Market data worker
    std::vector<std::string> watchlist = { "AAPL", "MSFT", "GOOGL", "TSLA"};
    worker_ = new MarketDataWorker(db_, watchlist, 10, this);
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

    // You could update a summary label here if you add one.
    (void)summary; // reserved for future UI
}

void MainWindow::onTickerChanged(const QString& ticker) {
    if (ticker.trimmed().isEmpty()) return;
    chart_->setTicker(ticker.toUpper());