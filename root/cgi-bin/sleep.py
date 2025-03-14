import time

print("Content-Type: text/html\n")

time.sleep(5)

print(f"""
<html>
<head>
    <title>Current Time</title>
</head>
<body>
    <h1>The program slept for 5 seconds!</h1>
</body>
</html>
""")