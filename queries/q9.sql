-- Simple filter fusion with high selectivity: No effect

-- All artists alive (end_date_year is NOT NULL) +
-- EXPLAIN ANALYZE select * from artist where end_date_year IS NULL;
EXPLAIN ANALYZE select * from artist where length(name) < 24 and id < 2000000;
-- none:            0.0204s
-- fpd:             0.0212s
-- fpd_sff:         0.0217s
-- fpd_ff:          0.0215s
-- fpd_ff_sb_0.3:   0.0214s

