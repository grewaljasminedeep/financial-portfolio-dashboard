#include "RealMarketDataWorker.h"
#include <pqxx/pqxx>
#include <QDebug>

RealMarketDataWorker::RealMarketDataWorker(DbConnection& db,
                                           const std::vector<std::string>& watchlist,
                                           const AlphaVantageConfig& avCfg,
                                           int pollIntervalSec,
                                           QObject* parent)
    : QObject(parent),
      db_(db),
      watchlist_(watchlist),
      avCfg_(avCfg),
      pollIntervalSec_(pollIntervalSec),
      client_(avCfg.api_key.isEmpty() ? "" : avCfg.api_key, avCfg.output_size, this),
      currentIndex_(0),
      running_(false)
{
    connect(&timer_, &QTimer::timeout, this, &RealMarketDataWorker::onTick);
    connect(&client_, &AlphaVantageClient::errorOccurred, this,
            [](const QString& ticker, const QString& msg) {
                qWarning() << "AlphaVantage error for" << ticker << ":" << msg;
            });
}

void RealMarketDataWorker::start() {
    if (running_) return;
    running_ = true;
    currentIndex_ = 0;
    timer_.start(pollIntervalSec_ * 1000);
    fetchNext(); // immediate first run
}

void RealMarketDataWorker::stop() {
    running_ = false;
    timer_.stop();
}

void RealMarketDataWorker::onTick() {
    if (!running_) return;
    fetchNext();
}

void RealMarketDataWorker::fetchNext() {
    if (watchlist_.empty()) return;
    if (currentIndex_ >= watchlist_.size()) {
        currentIndex_ = 0;
        emit newPricesAvailable();
    }

    QString ticker = QString::fromStdString(watchlist_[currentIndex_]);
    ++currentIndex_;

    client_.fetchDailyTimeSeries(ticker, [this] (const QString& ticker,
                                                 const std::vector<DailyBar>& bars,
                                                 bool success) {
                                                    onFetched(ticker, bars, success);
                                                 });
}

void RealMarketDataWorker::onFetched(const QString& ticker,
                                     const std::vector<DailyBar>& bars,
                                     bool success) {
                                        if (!success || bars.empty()) {
                                            return;
                                        }
                                        storeBars(ticker, bars);
                                        // Continue to next ticker asynchronously via timer
                                     }

void RealMarketDataWorker::storeBars(const QString& ticker,
                                     const std::vector<DailyBar>& bars) {
                                        pqxx::Connection& conn = db_.connection();
                                        pqxx::Work txn(conn);

                                        // Get asset_id for ticker
                                        std::string assetSql = "SELECT asset_id FROM assets WHERE ticker = $1";
                                        auto res = txn.exec_params(assetSql, ticker.toStdString());
                                        if (res.empty()) {
                                            qWarning() << "No asset found for ticker" << ticker;
                                            return;
                                        }
                                        int asset_id = res[0]["asset_id"].as<int>();

                                        //Upsert each bar
                                        std::string upsertSql =
                                            "INSERT INTO prices (asset_id, price_date, open_price, high_price, low_price, close_price, volume) "
                                            "VALUES ($1, $2, $3, $4, $5, $6, $7) "
                                            "ON CONFLICT (asset_id, price_date) DO UPDATE SET "
                                            "open_price = EXCLUDED.open_price, "
                                            "high_price = EXCLUDED.high_price, "
                                            "low_price = EXCLUDED.low_price, "
                                            "close_price = EXCLUDED.close_price, "
                                            "volume = EXCLUDED.volume";

                                        for (auto const& bar : bars) {
                                            QString dateStr = bar.date.toString("yyyy-MM-dd");
                                            txn.exec_params(upsertSql, 
                                                            asset_id, 
                                                            dateStr.toStdString(),
                                                            bar.open,
                                                            bar.high,
                                                            bar.low,
                                                            bar.close,
                                                            bar.volume);
                                        }

                                        txn.commit();
                                     }