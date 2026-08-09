#pragma once

#include "DbConnection.h"
#include <QThread>
#include <vector>
#include <string>

class MarketDataWorker : public QThread {
    Q_OBJECT
public:
    explicit MarketDataWorker(DbConnection& db,
        const std::vector<std::string>& watchlist,
        int pollIntervalSec,
        QObject* parent = nullptr);

    void stop();

signals:
    void newPricesAvailable();

protected:
    void run() override;

private:
    DbConnection& db_;
    std::vector<std::string> watchlist_;
    int pollIntervalSec_;
    bool running_;

    void fetchAndStoreMockPrices();
};