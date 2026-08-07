-- Assets table
CREATE TABLE IF NOT EXISTS assets (
    asset_id SERIAL PRIMARY KEY,
    ticker VARCHAR(10) UNIQUE NOT NULL,
    name VARCHAR(100),
    asset_class VARCHAR(20) NOT NULL,
    currency VARCHAR(3) NOT NULL DEFAULT 'USD'
);

-- Daily (or intraday) prices
CREATE TABLE IF NOT EXISTS prices (
    price_id SERIAL PRIMARY KEY,
    asset_id INT NOT NULL REFERENCES assets(asset_id) ON DELETE CASCADE,
    price_date DATE NOT NULL,
    open_price NUMERIC(12, 4),
    high_price NUMERIC(12, 4),
    low_price NUMERIC(12, 4),
    close_price NUMERIC(12, 4) NOT NULL,
    volume BIGINT,
    UNIQUE (asset_id, price_date)
);

-- Transactions (BUY / SELL)
CREATE TABLE IF NOT EXISTS transactions (
    transaction_id SERIAL PRIMARY KEY,
    asset_id INT NOT NULL REFERENCES assets(asset_id) ON DELETE CASCADE,
    transaction_type VARCHAR(4) NOT NULL CHECK (transaction_type IN ('BUY', 'SELL')),
    quantity NUMERIC(12, 4) NOT NULL,
    price_per_unit NUMERIC(12, 2) NOT NULL,
)