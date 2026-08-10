#include "MarketDataWorker.h"
#include <pqxx/pqxx>
#include <QDateTime>
#include <random>
#include <chrono>

MarketDataWorker::MarketDataWorker(DbConnection& db,
                                    const std::vector<std::string>& watchlist,
                                    int pollIntervalSec,
                                    QObject* parent) : QThread(parent), db_(db), watchlist_(watchlist),
                                    