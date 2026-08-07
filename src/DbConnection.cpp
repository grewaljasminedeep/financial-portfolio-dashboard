#include "DbConnection.h"
#include <stdexcept>
#include <string>
#include <memory>
#include <pqxx/pqxx>

DbConnection::DbConnection(const DbConfig& cfg) {
    std::string connInfo = "host=" + cfg.host +
                           " port=" + std::to_string(cfg.port) +
                           " dbname=" + cfg.db +
                           " user=" + cfg.user +
                           " password=" + cfg.password;

        conn_ = std::make_unique<pqxx::Connection>(connInfo.c_str());
        if (!conn_->is_open()) {
            throw std::runtime_error("Failed to open database connection");
        }
}

pqxx::Connection& DbConnection::connection() {
    return *conn_;
}