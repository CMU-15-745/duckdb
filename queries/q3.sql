-- Simple filter push-down

EXPLAIN ANALYZE select * from artist a inner join artist_credit_name ar on a.id = ar.artist_credit where begin_date_month = 1;

-- none:            0.141s
-- fpd:             0.0369s
-- fpd_sff:         0.0329s
-- fpd_ff:          0.0337s
-- fpd_ff_sb_0.3:   0.0330s