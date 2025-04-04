import sqlite3
import csv

conn = sqlite3.connect('players.db')

cursor = conn.cursor()

cursor.execute('''
CREATE TABLE IF NOT EXISTS players (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    player_id TEXT NOT NULL,
    player_name TEXT NOT NULL,
    nickname TEXT NOT NULL,
    age TEXT
)
''')

conn.commit()

csv_file = 'players.csv'

with open(csv_file, mode='r') as file:
    csv_reader = csv.DictReader(file)
    for row in csv_reader:
        cursor.execute('''
        INSERT INTO players (player_id, player_name, nickname, age)
        VALUES (?, ?, ?, ?)
        ''', (row['PLAYER_ID'], row['PLAYER_NAME'], row['NICKNAME'], row['AGE']))

conn.commit()

cursor.execute('SELECT * FROM players')

players = cursor.fetchmany(5)

for player in players:
    print(player)

conn.close()
