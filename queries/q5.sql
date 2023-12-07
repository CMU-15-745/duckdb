-- Multiple joins with a simple filter --> Massive reduction in amount of traffic
-- List the names of releases issued in vinyl format by the British band Coldplay.

explain analyze select
    distinct r1.name as rname
from
    artist_credit_name a1
        inner join artist_credit a2 on a1.artist_credit=a2.id
        inner join release r1 on a2.id=r1.artist_credit
        inner join release_info r2 on r1.id = r2.release
        inner join medium m1 on r1.id = m1.release
        inner join medium_format m2 on m1.format = m2.id
where
        a1.name='Coldplay' and m2.name like '%Vinyl'
order by
    date_year, date_month, date_day;

-- none:            0.531s
-- fpd:             0.151s
-- fpd_ff:          0.0506s
-- fpd_ff_sb_0.3:   0.0512s
