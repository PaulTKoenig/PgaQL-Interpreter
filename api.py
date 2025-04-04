import subprocess
import sqlite3
import json



# message = "CHART box_score IN scatter_plot FOR fga VS fgm WHERE team_abbr = 'CLE'"
# message = "CHART season_player_box_score IN scatter_plot FOR AVG pts VS SUM fgm WHERE team_abbr = 'CLE' AND player_id = '1627745'"

# process = subprocess.Popen(
#     ['./main'],
#     stdin=subprocess.PIPE,
#     stdout=subprocess.PIPE,
#     stderr=subprocess.PIPE,
#     text=True
# )

# response, errors = process.communicate(input=message)

# # Print the response for debugging
# print("Response:", response)
# print("Errors:", errors)

# # Parse the JSON response
# try:
#     result = json.loads(response)
#     if result["status"] == "success":
#         print("The process was successful:", result["message"])
#     else:
#         print("The process failed with error code", result["error_code"], ":", result["message"])
# except json.JSONDecodeError:
#     print("Failed to decode JSON response")

# query = result["message"]

query = "SELECT player_id, AVG(pts), SUM(fgm) FROM box_score WHERE team_abbr = 'CLE' GROUP BY player_id"

connection = sqlite3.connect('box_score.db')
cursor = connection.cursor()

cursor.execute(query)


results = cursor.fetchall()

for row in results:
    print(row)

cursor.close()
connection.close()