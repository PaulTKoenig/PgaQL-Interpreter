import subprocess
import sqlite3
import json



message = 'CHART box_score IN scatters_plot FOR mins VS fgm WHERE game_id = "0022300061"'

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
try:
    result = json.loads(response)
    if result["status"] == "success":
        print("The process was successful:", result["message"])
    else:
        print("The process failed with error code", result["error_code"], ":", result["message"])
except json.JSONDecodeError:
    print("Failed to decode JSON response")


connection = sqlite3.connect('box_score.db')
cursor = connection.cursor()

cursor.execute(response)


results = cursor.fetchall()

for row in results:
    print(row)

cursor.close()
connection.close()