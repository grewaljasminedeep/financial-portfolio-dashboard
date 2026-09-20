#include "AlphaVantageClient.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrlQuery>
#include <QDebug>

AlphaVantageClient::AlphaVantageClient(const QString& apiKey,
                                       const QString& outputSize,
                                       QObject* parent)
    :QObject(parent), apiKey_(apiKey), outputSize_(outputSize) {}

void AplhaVantageClient::fetchDailyTimeSeries(const QString& symbol, FetchCallback cb) {
    QUrl url("https://www.alphavantage.co/query");
    QUrlQuery query;
    query.addQueryItem("function", "TIME_SERIES_DAILY");
    query.addQueryItem("symbol", symbol);
    query.addQueryItem("outputsize", outputSize_);
    query.addQueryItem("apikey", apiKey_);
    url.setQuery(query);

    QNetworkRequest request(url);
    QNetworkReply* reply = manager_.get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, symbol, cb]() {
        onReplyFinished(symbol, cb, reply);
    });
}

void AlphaVantageClient::onReplyFinished(const QString& symbol, FetchCallback cb, QNetworkReply* reply) {
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred(symbol, reply->errorString());
        if (cb) cb(symbol, {}, false);
        return;
    }

    QByteArray data = reply->readAll();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJasonParseError::NoError) {
        emit errorOccurred(symbol, "JSON parse error: " + err.errorString());
        if (cb) cb(symbol, {}, false);
        return;
    }

    QJsonObject root = doc.object();

    // Check for error/info messages
    if (root.contains("Error Message")) {
        emit errorOccurred(symbol, root["Error Message"].toString());
        if (cb) cb(symbol, {}, false);
        return;
    }
    if (root.contains("Note")) {
        // Rate limit message etc.
        emit errorOccurred(symbol, "API note: " + root["Note"].toString());
        // Still try to parse if possible
    }

    QJsonObject ts = root["Time Series (Daily)"].toObject();
    if (ts.isEmpty()) {
        emit errorOccurred(symbol, "No time series data returned");
        if (cb) cb(symbol, {}, false);
        return;
    }

    std::vector<DailyBar> bars;
    for (auto it = ts.begin(); it != ts.end(); ++it) {
        QString dateStr = it.key(); // "YYYY-MM-DD"
        QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
        if (!date.isValid()) continue;

        QJsonObject day = it.value().toObject();

        auto getDouble = [&](const QString& key) -> double {
            return day[key].toString().toDouble();
        };
        auto getInt = [&](const QString& key) -> quint64 {
            return day[key].toString().toLongLong();
        };

        DailyBar bar;
        bar.date = date;
        bar.open = getDouble("1. open");
        bar.high = getDouble("2. high");
        bar.low = getDouble("3. low");
        bar.close = getDouble("4. close");
        bar.volume = getDouble("5. volume"); // or use toLongLong

        bars.push_back(bar);
    }

    // Sort ascending by date
    std::sort(bars.begin(), bars.end(), [](const DailyBar& a, const DailyBar& b) {
        return a.date < b.date;
    });

    if (cb) cb(symbol, bars, true);
}