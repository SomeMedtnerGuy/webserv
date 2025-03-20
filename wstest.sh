#!/bin/bash

urls=(
	"localhost:1234"
	"localhost:1235"
	"--resolve example1:1234:127.0.0.1 example1:1234"
	"--resolve example2:1235:127.0.0.1 example2:1235"
	"--resolve example1:1235:127.0.0.1 example1:1235"
	"--resolve example2:1234:127.0.0.1 example2:1234"
	"localhost:1234/doesNotExist"
	"localhost:1235/doesNotExist"
	"--data-binary @Makefile localhost:1234/Makefile1"
	"--data-binary @Makefile localhost:1235/Makefile2"


)

for url in "${urls[@]}"; do
	echo "Requesting: $url"
	curl $url
	echo -e "\n"
done
