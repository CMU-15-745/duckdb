SELECT grade from Students, (SELECT * from B.courseID = Students.courseID) WHERE Students.name = 'ABC';

SELECT *
  FROM customer
 WHERE (SELECT SUM(order_amount)
          FROM orders
         WHERE orders.customer_id = customer.customer_id
           ()) > 1000;


select *
    from (select 42) t(i),
         (select *
            from (select 142) t3(k),
                 (select k) t4(l));

i = 42, k = 142, l = 142

-- Bind SELECT_STMT;
-- Bind FROM_CLAUSE;
-- Bind JoinRef
-- Bind Left Side of JoinRef
-- Bind TableRef

-- LateralBinder :: ExpressionBinder

-- Bind Right Side of JoinRef





select * from (select 42) t(i)
where i in (select l from (select 42) t(l) ,
                          (select i * 2) t2(j),
                          (select i + j) t3(k)
            where k in (select l*3 from (select 42 l) t4(l) where k-i = j));

i = 42


-- Apr 4th '23 (2023-04-01): SQL Commands Test on sambinder: --

-- FAIL --
SELECT *
     FROM (SELECT 42) t(i),
          (SELECT *
               FROM (SELECT 142 k) t3(k),
                    (SELECT 1 WHERE k-i=100) t4(l));

i = 42, k = 142, l = 1

-- SUCCESS --
SELECT *
     FROM (SELECT 42) t1(i),
          (SELECT 22) t2(j),
          (SELECT 1 WHERE i+j=64) t3(l);

i = 42, j = 22, l = 1

-- SUCCESS --
SELECT *
     FROM (SELECT 64) t1(i),
          (SELECT 22) t2(j),
          (SELECT 1 WHERE i+j=64) t3(l);

No Rows

-- SUCCESS --
select *
     from (select 42) t1(i),
          (select 22) t2(j),
          (select i+j where i+j=64) t3(l);

i = 44, j = 22, l = 64

-- SUCCESS --
SELECT *
     FROM (SELECT 42) t(i),
          (SELECT i * 2) t2(j),
          (SELECT i + j) t3(k);

i = 42, j = 84, k = 126

-- SUCCESS --
SELECT *
     FROM (SELECT i,j,k
          FROM (SELECT 42) t(i),
               (SELECT i * 2) t2(j),
               (SELECT i + j) t3(k));

i = 42, j = 84, k = 126

-- SUCCESS --
EXPLAIN SELECT *
     FROM (SELECT i,j,k
          FROM (SELECT 42) t(i),
               (SELECT i * 2) t2(j),
               (SELECT i + j) t3(k));

i = 42, j = 84, k = 126

-- FAIL --
SELECT *
     FROM (SELECT 42) t(i),
          (SELECT *
               FROM (SELECT 142 k) t3(k),
                    (SELECT 1 WHERE i+k=0) t4(l));

No Rows

-- SUCCESS --
SELECT *
     FROM (SELECT *
               FROM (SELECT 42) t1(i),
                    (SELECT 22) t2(j),
                    (SELECT 1 WHERE i+j=64) t3(l));

i = 42, j = 22, l = 1

-- FAIL --
SELECT *
     FROM (SELECT *
          FROM (SELECT 42) t(i),
               (SELECT *
                    FROM (SELECT 142 k) t3(k),
                         (SELECT 1 WHERE k-i=100) t4(l)));

i = 42, j = 22, l = 1

-- FAIL --
SELECT *
FROM (SELECT *
      FROM (SELECT 42) t(i),
           (SELECT *
            FROM (SELECT 142 k) t3(k),
                 (SELECT 1 WHERE i-k=-100) t4(l)));

i = 42, j = 22, l = 1

-- FAIL --
SELECT *
FROM (SELECT 42) t(i),
     (SELECT *
      FROM (SELECT 142 k) t3(k),
           (SELECT *
               FROM (SELECT 242 l) t4(l),
                    (SELECT 1 WHERE i+l+k=426) t5(m)));

No Rows

select i
    from (select 42) t(i)
    where i*2 in (select j from (select i) t(l) ,
                     (select l+i) t2(j),
                     (select i+j) t3(k));

i = 42

SELECT *
FROM (SELECT 42) t(i)
WHERE i IN (SELECT k
            FROM (SELECT 42) t(k)
            WHERE k IN (SELECT l
                        FROM (SELECT 42 l) t4(l)
                        WHERE i-k IN (SELECT * FROM (SELECT i+5))
            ));

No Rows

SELECT *
     FROM (SELECT 42) t(i),
          (SELECT *
             FROM (SELECT 142 k) t3(k),
                  (SELECT 1 WHERE i+k=0) t4(l));

No Rows

-- DO NOT DELETE THESE, HEAVILY USED FOR TESTING
select *
from (select 42) t4(m),
     (select 64) t5(n)
where m in (
    select j from
    (select 21*m*n) t(i),
    (select m) t2(j));

m = 42, n = 64

SELECT *
FROM (SELECT 42) t4(m)
WHERE m IN (
    SELECT i FROM
                 (SELECT m) t(i),
                 (SELECT i*m/2 * 2/m) t2(j));

m = 42

-- DO NOT DELETE THESE, HEAVILY USED FOR TESTING

 SELECT *
 FROM (SELECT 10) t(i)
 WHERE i IN (SELECT k
             FROM (SELECT 20) t(k)
             WHERE k IN (SELECT l
                         FROM (SELECT 30) t4(l)
                         WHERE i-k IN (SELECT * FROM (SELECT i+100))
             ));

No Rows

 SELECT *
 FROM (SELECT 42) t(i)
 WHERE i IN (SELECT k
             FROM (SELECT 42) t(k)
             WHERE k IN (SELECT l
                         FROM (SELECT 42 l) t4(l)
                         WHERE i-k IN (SELECT * FROM (SELECT 0))
             ));

i = 42

 SELECT *
 FROM (SELECT 42) t(i)
 WHERE i IN (SELECT k
             FROM (SELECT 42) t(k)
             WHERE k IN (SELECT i
                         FROM (SELECT i) t4
                         WHERE i*2-k IN (SELECT * FROM (SELECT i))
             ));

i = 42

 SELECT *
 FROM (SELECT 42) t1(i),
      (SELECT 22) t2(j),
      (SELECT i+j WHERE i+j=(SELECT 64)) t3(l);

i = 42, j = 22, l = 64

 SELECT *
 FROM (SELECT 42) t(i)
 WHERE i IN (SELECT k
             FROM (SELECT i) t(k)
             WHERE k = 0);

No Rows

SELECT * FROM (SELECT 42) t(i)
WHERE i IN (SELECT l FROM (SELECT 42) t(l)
            WHERE l IN (SELECT l FROM (SELECT 42 l) t4(l),
                                       (SELECT l+5) t5(n)
                                        WHERE i+2*l IN (SELECT k FROM (SELECT i+5),
                                                                    (SELECT i * 2) t2(j),
                                                                    (SELECT i + j) t3(k)
                                                                WHERE k-l IN (SELECT i*2))
                                        ));
i = 42


SELECT (SELECT (SELECT k
                FROM (SELECT i) t2(j),
                     (SELECT j) t3(k)))
FROM (SELECT 42) t(i);
? = 42

select i
    from (select 42) t(i)
    where i in (select l
                    from (select 42) t(l) ,
                         (select 42) t2(j),
                         (select i+j+l) t3(k));
i = 42


