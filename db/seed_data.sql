-- Sample assets
INSERT INTO assets (ticker, name, asset_class, currency) VALUES
('AAPL', 'Apple Inc.', 'EQUITY', 'USD'),
('MSFT', 'Microsoft Corporation', 'EQUITY', 'USD'),
('GOOGL', 'Alphabet Inc.', 'EQUITY', 'USD'),
('TSLA', 'Tesla, Inc.', 'EQUITY', 'USD')
ON CONFLICT (ticker) DO NOTHING;

-- Sample transactions (adjust asset_id based on the DB; using subqueries for safety)
INSERT INTO transactions (asset_id, transaction_type, quantity, price_per_unit, executed_at)
SELECT a.asset_id, 'BUY', 10, 150.00, '2025-01-15 10:30:00+00'
FROM assets a WHERE a.ticker = 'AAPL';

INSERT INTO transactions (asset_id, transaction_type, quantity, price_per_unit, executed_at)
SELECT a.asset_id, 'BUY', 5, 300.00, '2025-02-10 11:00:00+00'
FROM assets a WHERE a.ticker = 'MSFT';

INSERT INTO transactions (asset_id, transaction_type, quantity, price_per_unit, executed_at)
SELECT a.asset_id, 'BUY', 8, 140.00, '2025-03-05 09:45:00+00'
FROM assets a WHERE a.ticker = 'GOOGL';