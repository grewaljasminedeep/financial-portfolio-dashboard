cmake_minimum_required(VERSION 3.16)
project(FinancialPortfolioDashboard VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBPQXX REQUIRED libpqxx)
find_package(yaml-cpp REQUIRED)

add_executable(FinancialPortfolioApp
    src/main.cpp
    src/DbConnection.cpp
    src/MarketDataWorker.cpp
    src/PortfolioEngine.cpp
    src/PortfolioModel.cpp
    src/ChartWidget.cpp
    src/MainWindow.cpp
    src/Config.cpp
)

target_include_directories(FinancialPortfolioApp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${LIBPQXX_INCLUDE_DIRS}
)

target_link_libraries(FinancialPortfolioApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    ${LIBPQXX_LIBRARIES}
    yaml-cpp
)

target_compile_options(FinancialPortfolioApp PRIVATE
    -Wall -Wextra -pedantic
)