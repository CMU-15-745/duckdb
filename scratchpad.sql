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
            from (select 142 k) t3(k),
                 (select k) t4(l)) t2(j);

-- Bind SELECT_STMT;
-- Bind FROM_CLAUSE;
-- Bind JoinRef
-- Bind Left Side of JoinRef
-- Bind TableRef

-- LateralBinder :: ExpressionBinder

-- Bind Right Side of JoinRef


SELECT * FROM (SELECT 42) t(i)
WHERE i IN (SELECT l FROM (SELECT 42) t(l) ,
                          (SELECT i * 2) t2(j),
                          (SELECT i + j) t3(k)
            WHERE k IN (SELECT * FROM (SELECT 42 l) t4(l) WHERE i-k = 0));


-- Apr 4th '23 (2023-04-01): SQL Commands Test on sambinder: --

-- FAIL --
SELECT *
     FROM (SELECT 42) t(i),
          (SELECT *
               FROM (SELECT 142 k) t3(k),
                    (SELECT 1 WHERE i+k=0) t4(l));

-- SUCCESS --
SELECT *
     FROM (SELECT 42) t1(i),
          (SELECT 22) t2(j),
          (SELECT 1 WHERE i+j=64) t3(l);

-- SUCCESS --
SELECT *
     FROM (SELECT 42) t1(i),
          (SELECT 22) t2(j),
          (SELECT 1 WHERE i=64) t3(l);

-- SUCCESS --
SELECT *
     FROM (SELECT 42) t1(i),
          (SELECT 22) t2(j),
          (SELECT i+j WHERE i+j=64) t3(l);

-- SUCCESS --
SELECT *
     FROM (SELECT 42) t(i),
          (SELECT i * 2) t2(j),
          (SELECT i + j) t3(k);

-- SUCCESS --
SELECT *
     FROM (SELECT i,j,k
          FROM (SELECT 42) t(i),
               (SELECT i * 2) t2(j),
               (SELECT i + j) t3(k));

-- SUCCESS --
EXPLAIN SELECT *
     FROM (SELECT i,j,k
          FROM (SELECT 42) t(i),
               (SELECT i * 2) t2(j),
               (SELECT i + j) t3(k));

-- FAIL --
SELECT *
     FROM (SELECT 42) t(i),
          (SELECT *
               FROM (SELECT 142 k) t3(k),
                    (SELECT 1 WHERE i+k=0) t4(l));
-- SUCCESS --
SELECT *
     FROM (SELECT *
               FROM (SELECT 42) t1(i),
                    (SELECT 22) t2(j),
                    (SELECT 1 WHERE i+j=64) t3(l));

-- FAIL --
SELECT *
     FROM (SELECT *
          FROM (SELECT 42) t(i),
               (SELECT *
                    FROM (SELECT 142 k) t3(k),
                         (SELECT 1 WHERE i+k=0) t4(l)));

SELECT *
FROM (SELECT *
      FROM (SELECT 42) t(i),
           (SELECT *
            FROM (SELECT 142 k) t3(k),
                 (SELECT 1 WHERE i=0) t4(l)));


SELECT *
FROM (SELECT 42) t(i),
     (SELECT *
      FROM (SELECT 142 k) t3(k),
           (SELECT *
               FROM (SELECT 242 l) t4(l),
                    (SELECT 1 WHERE i+l+k=0) t5(m)));


SELECT *
    FROM (SELECT 42) t(i)
    WHERE i IN (SELECT l
                FROM (SELECT 42) t(l) ,
                     (SELECT i * 2) t2(j),
                     (SELECT i + j) t3(k)
                WHERE k IN (SELECT l
                                  FROM (SELECT 42 l) t4(l),
                                       (SELECT l+5) t5(n)
                                  WHERE i-k IN (SELECT * FROM (SELECT i+5))
            ));

SELECT *
FROM (SELECT 42) t(i)
WHERE i IN (SELECT k
            FROM (SELECT 42) t(k)
            WHERE k IN (SELECT l
                        FROM (SELECT 42 l) t4(l)
                        WHERE i-k IN (SELECT * FROM (SELECT i+5))
            ));


SELECT *
     FROM (SELECT 42) t(i),
          (SELECT *
             FROM (SELECT 142 k) t3(k),
                  (SELECT 1 WHERE i+k=0) t4(l));

-- DO NOT DELETE THESE, HEAVILY USED FOR TESTING
SELECT *
FROM (SELECT 42) t4(m)
WHERE m IN (
    SELECT j FROM
    (SELECT 21*m) t(i),
    (SELECT m) t2(j));

SELECT *
FROM (SELECT 42) t4(m)
WHERE m IN (
    SELECT j FROM
    (SELECT 21*m) t(i),
    (SELECT i) t2(j));

-- DO NOT DELETE THESE, HEAVILY USED FOR TESTING

 SELECT *
 FROM (SELECT 10) t(i)
 WHERE i IN (SELECT k
             FROM (SELECT 20) t(k) WHERE k IN (SELECT l
                         FROM (SELECT 30) t4(l)
                         WHERE i-k IN (SELECT * FROM (SELECT i+100))
             ));

 SELECT *
 FROM (SELECT 42) t(i)
 WHERE i IN (SELECT k
             FROM (SELECT 42) t(k)
             WHERE k IN (SELECT l
                         FROM (SELECT 42 l) t4(l)
                         WHERE i-k IN (SELECT * FROM (SELECT 0))
             ));

 SELECT *
 FROM (SELECT 42) t(i)
 WHERE i IN (SELECT k
             FROM (SELECT 42) t(k)
             WHERE k IN (SELECT i
                         FROM (SELECT i) t4
                         WHERE i-k IN (SELECT * FROM (SELECT i))
             ));

 SELECT *
 FROM (SELECT 42) t1(i),
      (SELECT 22) t2(j),
      (SELECT i+j WHERE i+j=(SELECT 64)) t3(l);

 SELECT *
 FROM (SELECT 42) t(i)
 WHERE i IN (SELECT k
             FROM (SELECT i) t(k)
             WHERE k = 0);


SELECT * FROM (SELECT 42) t(i)
WHERE i IN (SELECT l FROM (SELECT 42) t(l)
            WHERE l IN (SELECT l FROM (SELECT 42 l) t4(l),
                                       (SELECT l+5) t5(n)
                                        WHERE i-l IN (SELECT k FROM (SELECT i+5),
                                                                    (SELECT i * 2) t2(j),
                                                                    (SELECT i + j) t3(k)
                                                                WHERE l-k IN (SELECT i+10))
                                        ));
