#include "Config.h"
#include <yaml-cpp/yaml.h>
#include <stdexcept>

AppConfig loadConfig(const std::string& path) {
    AppConfig cfg;
    YAML::Node root;
    try {
        root = YAML::LoadFile(path);
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Failed to load config file: ") + e.what());
    }

    if (auto db = root["database"]) {
        if (db["host"]) cfg.database.host = db["host"].as<std::string>();
        if (db["port"]) cfg.database.port = db["port"].as<int>();
        if (db["name"]) cfg.database.db = db["name"].as<std::string>();
        if (db["user"]) cfg.database.user = db["user"].as<std::string>();
        if (db["password"]) cfg.database.password = db["password"].as<std::string>();
    }

    if (auto md = root["market_data"]) {
        if (md["provider"]) cfg.market_data.provider = md["provider"].as<std::string>();
        if (md["api_key"]) cfg.market_data.api_key = md["api_key"].as<std::string>();
        if (md["poll_interval_seconds"]) cfg.market_data.poll_interval_seconds = md["poll_interval_seconds"].as<int>();
    }

    if (auto wl = root["watchlist"]) {
        cfg.watchlist = wl.as<std::vector<std::string>>();
    }

    return cfg
}