#pragma once

#include <QObject>
#include <QString>
#include <QDate>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <functional>
#include <vector>

struct DailyBar {
    QDate date;
    double open;
    double high;
    double low;
    double close;
    qint64 volume;
};

class AlphaVantageClient : public QObject {
    Q_OBJECT
public:
    explicit AlphaVantageClient(const QString& apiKey, 
                                const QString& outputSize = "compact",
                                QObject* parent = nullptr);

    using FetchCallback = std::function<void(const QString& ticker,
                                             const std::vector<DailyBar>&,
                                             bool success)>;

    void fetchDailyTimeSeries(const QString& symbol, FetchCallback cb);

signals:
    void errorOccurred(const QString& ticker, const QString& message);

private:
    QString apiKey_;
    QString outputSize_;
    QNetworkAccessManager manager_;

    void onReplyFinished(const QString& symbol, FetchCallback cb, QNetworkReply* reply);
};