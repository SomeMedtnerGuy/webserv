#!/usr/bin/env python3

import datetime

#<meta http-equiv="refresh" content="1">
# Define the HTTP header
print("Content-Type: text/html\n")

import os
import sys

query_string = os.environ.get("QUERY_STRING", "")
index = query_string.find("name")
if index != -1:
    name = query_string.split("name=")[-1].split("&")[0]  # Extract the value after 'name='
else:
    name = "Guest" 

now = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")

# Return the HTML with the updated time
print(f"""
<html>
<head>
    <title>Current Time</title>
</head>
<body>
    <h1>Server Current Time</h1>
    
    <p>{now}</p>
	
	<h2>Hello, {name}!</h2>  <!-- Display the extracted name -->
</body>
</html>
""")
sys.stdout.flush()