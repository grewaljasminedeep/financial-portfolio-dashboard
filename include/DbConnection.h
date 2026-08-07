#pragma once

#include <memory>
#include <string>
#include <pqxx/pqxx>

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
    pqxx::connection& connection();
private:
    std::unique_ptr<pqxx::connection> conn_;
};