# Financial Portfolio Dashboard

A C++17 desktop application for tracking and analyzing financial portfolios. Built with Qt6, PostgreSQL, libpqxx, and yaml-cpp, the dashboard provides portfolio monitoring, transaction management, and interactive price charts.

> The current market-data implementation uses the `mock` provider. External providers are planned but are not enabled yet.

## Features

- **Portfolio Management**: Track your investments across multiple assets
- **Transaction History**: Record and manage buy/sell transactions
- **Real-time Price Data**: Fetch market data from configurable providers
- **Interactive Charts**: Visualize price trends and portfolio performance
- **Database Persistence**: PostgreSQL backend for reliable data storage
- **Intuitive UI**: Qt6-based GUI for seamless user experience
- **Asset Tracking**: Monitor multiple stocks with configurable watchlists

## Prerequisites

Before building this project, ensure you have the following installed:

### Required
- **CMake** ≥ 3.16
- **C++ Compiler** with C++17 support (GCC, Clang, or MSVC)
- **Qt6** (Core, Gui, Widgets, Sql modules)
- **libpqxx** and its PostgreSQL client dependencies
- **yaml-cpp**
- **pkg-config** (used by CMake to locate libpqxx)
- **PostgreSQL** server (for database backend)

#### Ubuntu/Debian
```bash
sudo apt-get install cmake build-essential pkg-config qt6-base-dev libqt6sql6 libpqxx-dev libyaml-cpp-dev postgresql postgresql-contrib
```

#### macOS (using Homebrew)
```bash
brew install cmake pkg-config qt libpqxx yaml-cpp postgresql@15
```

#### Windows
- Download and install CMake from https://cmake.org/.
- Download Qt6 from https://www.qt.io/download.
- Install PostgreSQL from https://www.postgresql.org/download/windows/.
- Install `libpqxx`, `yaml-cpp`, and `pkg-config` using a package manager such as vcpkg or MSYS2.
- Install Visual Studio Build Tools or MinGW with C++17 support

## Building

1. **Clone the repository**
```bash
git clone <repository-url>
cd financial-portfolio-dashboard
```

2. **Create a build directory**
```bash
mkdir build
cd build
```

3. **Configure with CMake**
```bash
cmake ..
```

If CMake cannot locate Qt6 or another dependency, provide the installation prefix explicitly:

```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/Qt6
```

4. **Build the application**
```bash
cmake --build . --config Release
```

The compiled executable will be in the `build/` directory. On multi-configuration generators such as Visual Studio, it will typically be in `build/Release/`.

## Database Setup

1. **Create the database and user**
```bash
sudo -u postgres psql
```

```sql
CREATE DATABASE portfolio_db;
CREATE USER trader WITH PASSWORD 'secure_pass';
GRANT ALL PRIVILEGES ON DATABASE portfolio_db TO trader;
```

2. **Initialize the schema**
```bash
sudo -u postgres psql -d portfolio_db -f db/schema.sql
```

3. **Seed initial data (optional)**
```bash
sudo -u postgres psql -d portfolio_db -f db/seed_data.sql
```

## Configuration

Edit `config.yaml` to customize application settings:

```yaml
database:
  host: "localhost"           # PostgreSQL host
  port: 5432                  # PostgreSQL port
  name: "portfolio_db"        # Database name
  user: "trader"              # Database user
  password: "secure_pass"     # Database password

market_data:
  provider: "mock"            # Data provider: "mock", "alphavantage", "yahoo"
  api_key: ""                 # API key for market data provider
  poll_interval_seconds: 10   # Refresh interval for market data

watchlist:                     # Asset tickers to monitor
  - "AAPL"
  - "MSFT"
  - "GOOGL"
  - "TSLA"
```

The application looks for `config.yaml` in its current working directory. Run it from the repository root, or place a copy of the configuration file beside the executable.

## Running the Application

After successful build and database setup:

```bash
# Linux/macOS
./build/FinancialPortfolioApp

# Windows with a Visual Studio build
build\\Release\\FinancialPortfolioApp.exe
```

The application opens a `1100x650` window displaying the portfolio dashboard. If `config.yaml` is missing or invalid, the application logs a warning and uses default configuration values.

## Project Structure

```
financial-portfolio-dashboard/
├── CMakeLists.txt              # CMake build configuration
├── config.yaml                 # Application configuration
├── README.md                   # This file
├── include/                    # Header files
│   ├── ChartWidget.h           # Chart visualization component
│   ├── DbConnection.h          # Database connection manager
│   ├── MainWindow.h            # Main application window
│   ├── MarketDataWorker.h      # Market data fetching thread
│   ├── PortfolioEngine.h       # Portfolio calculation logic
│   └── PortfolioModel.h        # Portfolio data model
├── src/                        # Implementation files
│   ├── main.cpp                # Application entry point
│   ├── ChartWidget.cpp
│   ├── DbConnection.cpp
│   ├── MainWindow.cpp
│   ├── MarketDataWorker.cpp
│   ├── PortfolioEngine.cpp
│   └── PortfolioModel.cpp
├── db/                         # Database files
│   ├── schema.sql              # Database schema definition
│   └── seed_data.sql           # Sample data for testing
└── resources/                  # Application resources (icons, etc.)
```

## Key Components

### DbConnection
Manages PostgreSQL database connections and queries using libpqxx. Handles asset, price, and transaction data.

### MarketDataWorker
Background worker thread that refreshes market data at configurable intervals. The current implementation supports mock data and is structured for future provider integrations.

### PortfolioEngine
Core portfolio calculation engine. Computes metrics like total value, gains/losses, asset allocation, and performance analytics.

### PortfolioModel
Qt data model adapting portfolio data for UI display. Integrates with QTableView for transaction history and statistics.

### ChartWidget
Custom Qt widget for rendering price charts and portfolio visualizations using Qt's graphics framework.

### MainWindow
Main application window orchestrating all components and handling user interactions.

## Development

### Coding Standards
- C++17 standard
- Qt signals/slots for async operations
- Thread-safe database operations
- Compiler warnings enabled (-Wall -Wextra -pedantic)

### Building in Debug Mode
```bash
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

### Adding Market Data Providers
Extend `MarketDataWorker` with additional provider implementations:
1. Create provider-specific data fetching logic
2. Update `config.yaml` provider options
3. Implement provider selection in worker initialization

## Troubleshooting

### Database Connection Failed
- Verify PostgreSQL is running
- Check credentials in `config.yaml`
- Ensure database and schema are initialized
- Test connection: `psql -h localhost -U trader -d portfolio_db`

### Qt Libraries Not Found
- On Linux: `export LD_LIBRARY_PATH=/path/to/qt6/lib:$LD_LIBRARY_PATH`
- On macOS: Ensure Qt6 installation directory is in CMake's module path
- On Windows: Add Qt6 bin directory to system PATH

### CMake Configuration Issues
- Delete and recreate the `build` directory
- Regenerate: `cmake -DCMAKE_PREFIX_PATH=/path/to/Qt6 ..`

## Future Enhancements

- [ ] Multiple market data providers (Alpha Vantage, Yahoo Finance, etc.)
- [ ] Portfolio comparison and benchmarking
- [ ] Advanced analytics and reporting
- [ ] Mobile companion app
- [ ] Export functionality (PDF, CSV)
- [ ] Custom alerts and notifications

## Support

For issues, questions, or contributions, please open an issue or pull request on the project repository.

---

**Version**: 0.1.0  
**Last Updated**: 2026-08-25
