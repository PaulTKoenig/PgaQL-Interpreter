import sqlite3
import csv

conn = sqlite3.connect('player_stats.db')

cursor = conn.cursor()

cursor.execute('''
CREATE TABLE IF NOT EXISTS player_stats (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    firstName TEXT NOT NULL,
    lastName TEXT NOT NULL,
    playerId TEXT NOT NULL,
    gameId TEXT NOT NULL,
    gameDate TEXT NOT NULL,
    playerteamCity TEXT NOT NULL,
    playerteamName TEXT NOT NULL,
    opponentteamCity TEXT NOT NULL,
    opponentteamName TEXT NOT NULL,
    gameType TEXT NOT NULL,
    gameLabel TEXT,
    gameSubLabel TEXT,
    seriesGameNumber TEXT,
    win INTEGER,
    home INTEGER,
    mins REAL,
    pts REAL,
    ast REAL,
    blk REAL,
    stl REAL,
    fga REAL,
    fgm REAL,
    fg_pct REAL,
    three_pa REAL,
    three_pm REAL,
    three_pct REAL,
    fta REAL,
    ftm REAL,
    ft_pct REAL,
    d_reb REAL,
    o_reb REAL,
    reb REAL,
    personal_foul REAL,
    turnover REAL,
    plus_minus REAL
)
''')


conn.commit()

csv_file = 'PlayerStatistics2.csv'

with open(csv_file, mode='r') as file:
    csv_reader = csv.DictReader(file)
    for row in csv_reader:
        cursor.execute('''
            INSERT INTO player_stats (
                firstName, lastName, playerId, gameId, gameDate, playerteamCity, playerteamName, 
                opponentteamCity, opponentteamName, gameType, gameLabel, gameSubLabel, seriesGameNumber,
                win, home, mins, pts, ast, blk, stl, fga, fgm, fg_pct, 
                three_pa, three_pm, three_pct, fta, ftm, ft_pct, d_reb, o_reb, reb, 
                personal_foul, turnover, plus_minus
            ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        ''', (
            row['firstName'], row['lastName'], row['personId'], row['gameId'], row['gameDate'],
            row['playerteamCity'], row['playerteamName'], row['opponentteamCity'], row['opponentteamName'],
            row['gameType'], row.get('gameLabel', None), row.get('gameSubLabel', None), row.get('seriesGameNumber', None),
            row['win'], row['home'], row['numMinutes'],
            row['points'], row['assists'], row['blocks'], row['steals'],
            row['fieldGoalsAttempted'], row['fieldGoalsMade'], row['fieldGoalsPercentage'],
            row['threePointersAttempted'], row['threePointersMade'], row['threePointersPercentage'],
            row['freeThrowsAttempted'], row['freeThrowsMade'], row['freeThrowsPercentage'],
            row['reboundsDefensive'], row['reboundsOffensive'], row['reboundsTotal'],
            row['foulsPersonal'], row['turnovers'], row['plusMinusPoints']
        ))

conn.commit()

cursor.execute('SELECT * FROM player_stats')

player_stats = cursor.fetchmany(5)

for player_stat in player_stats:
    print(player_stat)

conn.close()
