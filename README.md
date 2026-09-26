# Financial Portfolio Dashboard

A Qt-based C++ desktop application for tracking portfolio performance, monitoring watchlists, and visualizing asset price history from PostgreSQL-backed data.

This project combines:
- Qt6 widgets for the interactive dashboard
- PostgreSQL + libpqxx for persistent portfolio and market data storage
- YAML config files for app and database settings
- Mock and Alpha Vantage market-data providers
- Custom portfolio analytics and chart rendering

## Overview

The dashboard shows a summary of total portfolio value, cost basis, and unrealized P&L, along with a table of positions and a chart for the selected ticker. The application loads configuration from `config.yaml`, connects to PostgreSQL, and periodically refreshes market data for each symbol in the configured watchlist.

## Features

- Portfolio summary panel with market value and P&L
- Asset position table for holdings and transactions
- Ticker-based chart view in the UI
- Configurable watchlists
- PostgreSQL schema for assets, prices, and transactions
- Mock data provider for local/offline development
- Alpha Vantage-backed real market-data polling
- CMake-based cross-platform build setup

## Tech Stack

- C++17
- Qt6 Core / Gui / Widgets / Sql / Network
- PostgreSQL
- libpqxx
- yaml-cpp
- CMake 3.16+

## Prerequisites

Install the following before building:

- CMake 3.16 or newer
- A C++17 compiler (GCC, Clang, or MSVC)
- Qt6 development libraries
- PostgreSQL server and client libraries
- libpqxx with PostgreSQL client headers
- yaml-cpp
- pkg-config

### Ubuntu / Debian

```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  cmake \
  pkg-config \
  qt6-base-dev \
  libqt6sql6 \
  libpqxx-dev \
  libyaml-cpp-dev \
  postgresql postgresql-contrib
```

### macOS (Homebrew)

```bash
brew install cmake pkg-config qt libpqxx yaml-cpp postgresql@15
```

### Windows

On Windows, install:
- CMake from https://cmake.org/
- Qt6 from https://www.qt.io/download
- PostgreSQL from https://www.postgresql.org/download/windows/
- libpqxx and yaml-cpp via vcpkg, MSYS2, or another package manager
- Visual Studio Build Tools or MinGW with C++17 support

## Project Structure

```text
financial-portfolio-dashboard/
├── CMakeLists.txt
├── README.md
├── config.yaml
├── db/
│   ├── schema.sql
│   └── seed_data.sql
├── include/
│   ├── AlphaVantageClient.h
│   ├── ChartWidget.h
│   ├── Config.h
│   ├── DbConnection.h
│   ├── MainWindow.h
│   ├── MarketDataWorker.h
│   ├── PortfolioEngine.h
│   ├── PortfolioModel.h
│   └── RealMarketDataWorker.h
├── resources/
├── src/
│   ├── AlphaVantageClient.cpp
│   ├── ChartWidget.cpp
│   ├── Config.cpp
│   ├── DbConnection.cpp
│   ├── MainWindow.cpp
│   ├── MarketDataWorker.cpp
│   ├── PortfolioEngine.cpp
│   ├── PortfolioModel.cpp
│   ├── RealMarketDataWorker.cpp
│   ├── main.cpp
│   └── ...
└── build/
```

## Configuration

The application reads settings from `config.yaml` in the working directory. A sample configuration looks like this:

```yaml
database:
  host: "localhost"
  port: 5432
  name: "portfolio_db"
  user: "trader"
  password: "secure_pass"

market_data:
  provider: "mock"
  poll_interval_seconds: 10
  alphavantage:
    api_key: "YOUR_ALPHA_VANTAGE_KEY"
    output_size: "compact"

watchlist:
  - "AAPL"
  - "MSFT"
  - "GOOGL"
  - "TSLA"
```

Notes:
- `provider` supports `mock` and `alphavantage`.
- If `provider` is set to `alphavantage`, the app uses the nested `market_data.alphavantage.api_key` value.
- The app defaults to the mock provider when the configured provider is not active or no real-data source is available.

## Database Setup

The app expects a PostgreSQL database with the schema defined in `db/schema.sql`.

1. Create a database and user:

```sql
CREATE DATABASE portfolio_db;
CREATE USER trader WITH PASSWORD 'secure_pass';
GRANT ALL PRIVILEGES ON DATABASE portfolio_db TO trader;
```

2. Initialize the schema:

```bash
psql -h localhost -U trader -d portfolio_db -f db/schema.sql
```

3. Optional: load sample data:

```bash
psql -h localhost -U trader -d portfolio_db -f db/seed_data.sql
```

## Building

From the repository root:

```bash
cmake -S . -B build
cmake --build build --config Release
```

If Qt is not automatically detected, point CMake at the Qt install location:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt6
cmake --build build --config Release
```

The executable produced by this project is:

```text
build/FinancialPortfolioApp
```

On Windows, the output may appear under a build configuration directory such as `build/Release/FinancialPortfolioApp.exe`.

## Running the App

After building and setting up PostgreSQL:

```bash
./build/FinancialPortfolioApp
```

On Windows:

```powershell
build\Release\FinancialPortfolioApp.exe
```

When launched, the app opens a dashboard window with:
- total portfolio value
- cost basis
- unrealized P&L
- positions table
- ticker chart panel

## Market Data Behavior

The application has two main runtime data modes:

1. `mock`
   - Uses the mock worker to generate or simulate market data.
   - Useful for local development and testing without live API access.

2. `alphavantage`
   - Uses the real market-data worker and the Alpha Vantage client.
   - Pulls daily time series data for all symbols in the watchlist.
   - Stores price bars in the `prices` table keyed by asset and date.

## Database Schema

The application stores data in PostgreSQL tables including:

- `assets`
- `prices`
- `transactions`

The schema includes unique constraints and indexes for efficient lookups by ticker and date.

## Development Notes

- The app is designed around Qt signals and slots.
- Market refreshes are handled by worker objects and timers.
- The portfolio calculation logic lives in the portfolio engine and is reused by the UI.
- The UI uses a `QTableView` and a custom chart widget for position and price visualization.

## Troubleshooting

### Qt not found during CMake configure

Set the Qt prefix explicitly:

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/Qt6
```

### PostgreSQL connection fails

Verify:
- PostgreSQL is running
- `config.yaml` credentials match the database
- the schema has been applied with `db/schema.sql`

### Missing config file

Run the app from the project root, or place a valid `config.yaml` next to the executable.

## Future Improvements

Possible enhancements include:
- additional market data providers
- richer portfolio analytics and reporting
- transaction entry and editing in the UI
- alerts and watchlist notifications
- CSV/PDF export
- deeper historical analysis and benchmarking


---

Version: 0.1.0
