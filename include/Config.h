#pragma once

#include <string>
#include <vector>

struct DbConfig {
    std::string host = "localhost";
    int port =5432;
    std::string db = "portfolio_db";
    std::string user = "trader";
    std::string password = "secure_pass";
};

struct MarketDataConfig {
    std::string provider = "mock";
    std::string api_key;
    int poll_interval_seconds = 10;
};

struct AppConfig {
    DbConfig database;
    MarketDataConfig market_data;
    std::vector<std::string> watchlist;
};

AppConfig loadConfig(const std::string& path);