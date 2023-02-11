#!/usr/bin/env python3
# -*- coding: utf-8 -*-

###
# Récupération des données
# Stockage des donnés dans sqlite3
###

import requests
import time
import json
import sqlite3

response = requests.get("https://opendata.paris.fr/api/records/1.0/search/?dataset=velib-disponibilite-en-temps-reel&q=&facet=name&facet=is_installed&facet=is_renting&facet=is_returning&facet=nom_arrondissement_communes")
contenu = response.json()


conn = sqlite3.connect('/var/db_velib/velibdata.db')
cur = conn.cursor()
print("connexion réussie à SQLite")
 
for i in range(len(contenu['records'])):
    sql = "INSERT INTO station_information (capacity, lat, lon, name, station_id) VALUES ("
    sql = sql \
    +str(contenu['records'][i]['fields']['capacity'])+"," \
    +str(contenu['records'][i]['fields']['coordonnees_geo'][0])+"," \
    +str(contenu['records'][i]['fields']['coordonnees_geo'][1])+",'" \
    +str(contenu['records'][i]['fields']['name']).replace("'", " ")+"'," \
    +str(contenu['records'][i]['fields']['stationcode'])+")"
    print(sql)
 
    count = cur.execute(sql)
    print("enregistrement inséré dans table station_information")

for i in range(len(contenu['records'])):
    sql = "INSERT INTO station_status (station_id,is_installed,is_renting,is_returning,numBikesAvailable,numDocsAvailable,num_bikes_available,num_bikes_available_ebike,num_bikes_available_mechanical,last_reported)  VALUES ("+str(contenu['records'][i]['fields']['stationcode'])+",'"+str(contenu['records'][i]['fields']['is_installed'])+"','"+str(contenu['records'][i]['fields']['is_renting'])+"','"+str(contenu['records'][i]['fields']['is_returning'])+"',"+str(contenu['records'][i]['fields']['numbikesavailable'])+","+str(contenu['records'][i]['fields']['numdocksavailable'])+","+str(contenu['records'][i]['fields']['numbikesavailable'])+","+str(contenu['records'][i]['fields']['ebike'])+","+str(contenu['records'][i]['fields']['mechanical'])+",'"+str(contenu['records'][i]['fields']['duedate'])+"')"
    print(sql)
    
    count = cur.execute(sql)
    print("enregistrement inséré dans table station_status")

conn.commit()
cur.close()
conn.close()


