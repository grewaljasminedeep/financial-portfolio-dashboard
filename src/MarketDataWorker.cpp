#include "MarketDataWorker.h"
#include <pqxx/pqxx>
#include <QDateTime>
#include <random>
#include <chrono>

MarketDataWorker::MarketDataWorker(DbConnection& db,
                                    const std::vector<std::string>& watchlist,
                                    int pollIntervalSec,
                                    QObject* parent) : QThread(parent), db_(db), watchlist_(watchlist),
                                    pollIntervalSec_(pollIntervalSec),
                                    running_(true) {}

void MarketDataWorker::stop() {
    running_ = false;
    }

void MarketDataWorker::run() {
    while (running_) {
        fetchAndStoreMockPrices();
        emit newPricesAvailable();
        for (int i = 0; i < pollIntervalSec_ * 10 && running_; ++i){
             msleep(100);
             }
        }
    }

void MarketDataWorker::fetchAndStoreMockPrices() {
    pqxx::Connection& conn = db_.connection();
    pqxx::Work txn(conn);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> d(-0.02, 0.02);
    for (auto const& ticker : watchlist_){
        // Get last price for this ticker
        std::string sql = "SELECT p.close_price, a.asset_id"
                            "FROM prices p"
                            "JOIN assets a ON a.asset_id = p.asset_id"
                            "WHERE a.ticker = $1"
                            "ORDER BY p.price_date DESC"
                            "LIMIT 1";
        pqxx::Result res = txn.exec_params(sql, ticker);
        if (res.empty()) continue;
        double last_price = res[0] ["close_price"].as<double>();
        int asset_id = res[0] ["asset_id"].as<int>();
        double change = d(gen);
        double new_price = last_price * (1.0 + change);
        if (new_price < 0.01) new_price = 0.01;
    }
}