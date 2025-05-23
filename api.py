import subprocess
import sqlite3
import json



# message = "CHART box_score IN scatter_plot FOR fga VS fgm WHERE team_abbr = 'CLE'"
message = "CHART box_score IN scatter_plot FOR three_pa VS fg_pct WHERE blk = '10'"
# message = "CHART box_score IN scatter_plot FOR fga VS fgm WHERE team_abbr = 'CLE'"
process = subprocess.Popen(
    ['./main'],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    text=True
)

response, errors = process.communicate(input=message)

# Print the response for debugging
print("Response:", response)
print("Errors:", errors)

# Parse the JSON response
# try:
#     result = json.loads(response)
#     if result["status"] == "success":
#         print("The process was successful:", result["message"])
#     else:
#         print("The process failed with error code", result["error_code"], ":", result["message"])
# except json.JSONDecodeError:
#     print("Failed to decode JSON response")
# exit()
result = json.loads(response)
query = result["message"]
print(query)

# query = "SELECT player_id, AVG(pts), SUM(fgm) FROM box_score WHERE team_abbr = 'CLE' AND player_id = '1627745' GROUP BY player_id"

connection = sqlite3.connect('player_stats.db')
cursor = connection.cursor()

cursor.execute(query)


results = cursor.fetchall()

for row in results:
    print(row)

cursor.close()
connection.close()