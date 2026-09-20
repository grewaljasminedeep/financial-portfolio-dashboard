#pragma once

#include <string>
#include <vector>

struct DbConfig {
    std::string host = "localhost";
    int port = 5432;
    std::string db = "portfolio_db";
    std::string user = "trader";
    std::string password = "secure_pass";
};

struct AlphaVantageConfig{
    std::string api_key;
    std::string output_size = "compact"; // or "full"
};

struct MarketDataConfig {
    std::string provider = "mock"; // "mock" | "alphavantage"
    std::string api_key; // legacy/generic field
    int poll_interval_seconds = 10;
    AlphaVantageConfig alphavantage;
};

struct AppConfig {
    DbConfig database;
    MarketDataConfig market_data;
    std::vector<std::string> watchlist;
};

AppConfig loadConfig(const std::string& path);