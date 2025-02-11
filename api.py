import subprocess
import sqlite3



message = 'CHART box_score IN scatter_plot FOR mins VS fgm WHERE game_id = "0022300061"'

process = subprocess.Popen(
    ['./main'],
    stdin=subprocess.PIPE,
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    text=True
)

response, errors = process.communicate(input=message)

print("Received from C app:", response)



connection = sqlite3.connect('box_score.db')
cursor = connection.cursor()

cursor.execute(response)


results = cursor.fetchall()

for row in results:
    print(row)

cursor.close()
connection.close()