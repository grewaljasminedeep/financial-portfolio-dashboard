#pragma once

#include <pqxx/pqxx>
#include <string>
#include <memory>

struct DbConfig {
    std::string host = "localhost";
    int port = 5432;
    std::string db = "portfolio_db";
    std::string user = "trader";
    std::string password = "secure_pass";
};

class DbConnection {
    public:
    explicit DbConnection(const DbConfig& cfg);
    pqxx::Connection& connection();
    private:
}