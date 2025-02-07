import sqlite3
import csv

conn = sqlite3.connect('box_score.db')

cursor = conn.cursor()

cursor.execute('''
CREATE TABLE IF NOT EXISTS box_score (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    game_id TEXT NOT NULL,
    team_id TEXT NOT NULL,
    team_abbreviation TEXT NOT NULL,
    team_city TEXT NOT NULL,
    player_id TEXT NOT NULL,
    player_name TEXT NOT NULL,
    nickname TEXT NOT NULL,
    start_position TEXT,
    comment TEXT,
    min REAL,
    fgm REAL,
    fga REAL,
    fg_pct REAL,
    three_pm REAL,
    three_pa REAL,
    three_pct REAL,
    ftm REAL,
    fta REAL,
    ft_pct REAL,
    o_reb REAL,
    d_reb REAL,
    reb REAL,
    ast REAL,
    stl REAL,
    blk REAL,
    turnover REAL,
    pf REAL,
    pts REAL,
    plus_minus REAL
)
''')

conn.commit()

csv_file = 'nba_box_scores_2023_24.csv'

with open(csv_file, mode='r') as file:
    csv_reader = csv.DictReader(file)
    for row in csv_reader:
        cursor.execute('''
        INSERT INTO box_score (game_id, team_id, team_abbreviation, team_city, player_id, player_name,
                               nickname, start_position, comment, min, fgm, fga, fg_pct, three_pm, three_pa, three_pct, 
                               ftm, fta, ft_pct, o_reb, d_reb, reb, ast, stl, blk, turnover, pf, pts, plus_minus)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
        ''', (row['GAME_ID'], row['TEAM_ID'], row['TEAM_ABBREVIATION'], row['TEAM_CITY'], row['PLAYER_ID'],
              row['PLAYER_NAME'], row['NICKNAME'], row['START_POSITION'], row['COMMENT'], row['MIN'], 
              row['FGM'], row['FGA'], row['FG_PCT'], row['FG3M'], row['FG3A'], row['FG3_PCT'],
              row['FTM'], row['FTA'], row['FT_PCT'], row['OREB'], row['DREB'], row['REB'], row['AST'],
              row['STL'], row['BLK'], row['TO'], row['PF'], row['PTS'], row['PLUS_MINUS']))

conn.commit()

cursor.execute('SELECT * FROM box_score')

box_scores = cursor.fetchmany(5)

for box_score in box_scores:
    print(box_score)

conn.close()
