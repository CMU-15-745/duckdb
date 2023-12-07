-- List the top 10 dubbed artist names with the number of dubs.

-- Details: Count the number of distinct names in artist_alias for each artist in the artist table,
-- and list only the top ten who was born in the United Kingdom after 1950. Print the artist name in
-- the artist table and the number of corresponding distinct dubbed artist names in the artist_alias
-- table.

explain analyze Select artist.name, count(distinct artist_alias.name) as num
From artist inner join artist_alias on artist.id=artist_alias.artist
Where artist.begin_date_year > 1950 and area=221
Group by artist.id, artist.name
Order by num desc;

-- none             0.0453s
-- fpd:             0.0333s
-- fpd + ff:        0.0228s
-- fpd+ff+sb0.3:    0.0230s
