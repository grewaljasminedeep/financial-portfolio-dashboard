#pragma once

#include <pqxx/pqxx>
#include <string>
#include <memory>

struct DbConfig {
    std::string host = "localhost";
    int port = 5432;
};