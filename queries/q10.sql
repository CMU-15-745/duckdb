-- Very few filters with join --> Very little speedup and hash join time dominates
-- List the number of artists who have appeared in any release with Ariana Grande.

explain analyze Select count(distinct artist)
From artist_credit_name
Where artist_credit in
      (select artist_credit from artist_credit_name where name='Ariana Grande');


-- none:             0.0240s
-- fpd:              0.0243s
-- fpd_ff:          0.0238s
-- fpd_ff_sb_0.3:  0.0234s