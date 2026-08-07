-- Sample assets
INSERT INTO assets (ticker, name, asset_class, currency) VALUES
('AAPL', 'Apple Inc.', 'EQUITY', 'USD'),
('MSFT', 'Microsoft Corporation', 'EQUITY', 'USD'),
('GOOGL', 'Alphabet Inc.', 'EQUITY', 'USD'),
('TSLA', 'Tesla, Inc.', 'EQUITY', 'USD')
ON CONFLICT (ticker) DO NOTHING;

-- Sample transactions (adjust asset_id based on the DB; using subqueries for safety)
INSERT INTO transactions (asset_id, transaction_type, quantity, price_per_unit, executed_at)