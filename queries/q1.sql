-- Simple query with multiple simple filter fusion (low selectivity)

EXPLAIN ANALYZE select * from artist where begin_date_year = 2010 and area < 20;

-- none:            0.0180s
-- fpd:             0.0174s
-- fpd_sff:         0.0051s
-- fpd_ff:          0.0058s
-- fpd_ff_sb_0.3:   0.0044s