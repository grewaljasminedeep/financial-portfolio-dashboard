#pragma once

#include "DbConnection.h"
#include "AlphaVantageClient.h"
#include <QObject>
#include <QTimer>
#include <vector>
#include <string>

class RealMarketDataWorker : public QObject {
    Q_OBJECT
public:
    explicit RealMarketDataWorker(DbConnection& db,
                                  const std::vector<std::string>& watchlist,
                                  const AlphaVantageConfig& avCfg,
                                  int pollIntervalSec,
                                  QObject* parent = nullptr);

    void start();
    void stop();

signals:
    void newPricesAvailable();

private slots:
    void onTick();
    void onFetched(const QString& ticker,
                   const std::vector<DailyBar>& bars,
                   bool success);

private:
    DbConnection& db_;
    std::vector<std::string> watchlist_;
    AlphaVantageConfig avCfg_;
    int pollIntervalSec_;
    QTimer timer_;
    AlphaVantageClient client_;

    size_t currentIndex_;
    bool running_;

    void fetchNext();
    void storeBars(const QString& ticker, const std::vector<DailyBar>& bars);
};