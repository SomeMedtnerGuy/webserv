#!/usr/bin/env python3

import sys
import os
import cgi

# Defina o cabeçalho da resposta HTTP
print("Content-Type: text/html\n")

# Ler o corpo da requisição POST diretamente do stdin
content_length = int(os.environ.get('CONTENT_LENGTH', 0))  # Tamanho do corpo da requisição
post_data = sys.stdin.read(content_length)  # Leia os dados do corpo

# Exibir os dados recebidos na resposta HTML
print(f"""
<html>
<head>
    <title>Received POST Data</title>
</head>
<body>
    <h1>Received Data:</h1>
    <p>{post_data}</p>
</body>
</html>
""")
# print(post_data)

sys.stdout.flush()

# curl -X POST --data "Hello World" http://localhost:1234/cgi-bin/cgi-post.py
