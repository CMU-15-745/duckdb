-- Not alot of benefit because medium format does not have alot of columns

explain analyze select
    r.name,
    a.name,
    rinfo.date_year
from
    medium m
        inner join medium_format f on m.format = f.id
        inner join release r on r.id = m.release
        inner join release_info rinfo on rinfo.release = r.id
        inner join artist_credit ac on ac.id = r.artist_credit
        inner join artist_credit_name acn on acn.artist_credit = ac.id
        inner join artist a on a.id = acn.artist
where
        f.name LIKE 'Cassette'
order by
    rinfo.date_year desc,
    rinfo.date_month desc,
    rinfo.date_day desc,
    r.name,
    a.name;


-- none:            0.602s
-- fpd:             0.110s
-- fpd_ff:           0.123s
-- fpd_ff_sb_0.3:   0.0863s