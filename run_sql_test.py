import duckdb
import json

duckdb_file = "db.duckdb"
duckdb_query = "queries/q1.sql"

con = duckdb.connect(duckdb_file)
with open(duckdb_query, "r") as sql_file:
    sql_query = sql_file.read()

con.sql("PRAGMA enable_profiling='json'")
# print(sql_query)
res = con.sql(sql_query).to_df()
print(res['explain_value'][0])
res = (res['explain_value'][0])

res = json.loads(res)
total_time = res["timing"]
# seq_time = res["children"]
print(f"Total Time: {total_time}")

con.close()
