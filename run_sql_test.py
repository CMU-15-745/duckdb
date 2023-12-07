import duckdb
import json

import os


duckdb_file = "db_small.duckdb"
duckdb_query_path = "queries"
result_file = "results/res.txt"


RUN_COUNT = 13
WARMUP_COUNT = 2

with open(result_file, "w") as res_file:

  for query_file in sorted(os.listdir(duckdb_query_path)):
    con = duckdb.connect(duckdb_file)
    duckdb_query = os.path.join(duckdb_query_path, query_file)
    with open(duckdb_query, "r") as sql_file:
      sql_query = sql_file.read()

    con.sql("PRAGMA enable_profiling='json'")
    # Run the query a few times to warm up memory

    # print("Warmup")
    for _ in range(WARMUP_COUNT):
      con.sql(sql_query)

    times = []

    # print("Running test")
    for i in range(RUN_COUNT):
      output = con.sql(sql_query).to_df()
      res_str = output['explain_value'][0]
      res_dict = json.loads(res_str)
      times += [float(res_dict["timing"])]

    times = sorted(times)
    print("Query file: {}, min_time: {}, max_time: {}, avg_time: {}, median_time: {}".format(query_file, times[0], times[-1], sum(times)/RUN_COUNT, times[int(RUN_COUNT/2)]))
    res_file.write("{},{},{},{},{}\n".format(query_file, times[0], times[-1], sum(times)/RUN_COUNT, times[int(RUN_COUNT/2)]))
    con.close()

#  print(res['explain_value'][0])
