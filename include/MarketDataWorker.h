#pragma once

#include "DbConnection.h"
#include <QThread>
#include <vector>
#include <string>

class MarketDataWorker : public QThread {
    Q_OBJECT
}