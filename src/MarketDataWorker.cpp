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
}