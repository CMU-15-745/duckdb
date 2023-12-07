-- Complex query with multiple simple filter fusion (low selectivity)


EXPLAIN ANALYZE select * from artist where name like 'Michael%' and length(comment)=4;

-- none:            0.0183s
-- fpd:             0.0187s
-- fpd_sff:         0.0187s
-- fpd_ff:          0.0136s
-- fpd_ff_sb_0.3:   0.0130s