#!/usr/bin/env python3
# -*- coding: utf-8 -*-

##
#récupération données api vlib
#ajout time pour voir que tous les données ont bien été récupérés
##

import requests
import time
import json

url1 = "https://velib-metropole-opendata.smoove.pro/opendata/Velib_Metropole/station_status.json"
url2 = "https://velib-metropole-opendata.smoove.pro/opendata/Velib_Metropole/system_information.json"
url3 = "https://velib-metropole-opendata.smoove.pro/opendata/Velib_Metropole/station_information.json"

"""response = requests.get(url1)
contenu = response.json()
#print(contenu)
print(json.dumps(contenu, indent = 4, sort_keys=False))
print("Type:", type(contenu))
time.sleep(3)"""

"""response = requests.get(url2)
contenu = response.json()
#print(contenu)
print(json.dumps(contenu, indent = 4, sort_keys=False))
time.sleep(3)"""

response = requests.get(url3)
contenu = response.json()
print(json.dumps(contenu, indent = 4, sort_keys=False))
print("Type:", type(contenu))
#print(contenu)
