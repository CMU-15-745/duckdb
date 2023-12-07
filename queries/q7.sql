-- List top 10 countries with the most classical music artists (born or started before 1850) along
-- with the number of associated artists.

explain analyze select
    a2.name,
    count(*) as c
from
    area a2 join artist a1 on a1.area = a2.id
where
    begin_date_year < 1850
group by
    a2.name
order by
    c
desc;


-- none:            0.0244s
-- fpd:             0.0163s
-- fpd_ff:          0.0173s
-- fpd_ff_sb_0.3:   0.0163s
