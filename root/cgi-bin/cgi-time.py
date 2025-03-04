#!/usr/bin/env python3

import datetime

#<meta http-equiv="refresh" content="1">
# Definir o cabeçalho HTTP
#print("Content-Type: text/html\n")

import os
import sys

query_string = os.environ.get("QUERY_STRING", "")
index = query_string.find("name")
name = query_string.split("name=")[-1].split("&")[0]
# Obter a hora atual
now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

# Retornar o HTML com a hora atualizada
print(f"""
<html>
<head>
    <title>Hora Atual</title>
</head>
<body>
    <h1>Hora Atual do Servidor para atualizar o(a) {name}</h1>
    
    <p>{now}</p>
</body>
</html>
""")
sys.stdout.flush()