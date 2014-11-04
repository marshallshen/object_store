#!/bin/bash
yellow='\033[0;33m'
NC='\e[0m'

echo -e "${yellow}[Before Test] Compile targeted files..${NC}"
gcc -std=c99 -Wall -g -o program crypto_sample.c -lssl -lcrypto -Wno-pointer-sign
./program
echo -e "${yellow}[After Test] Clean up compiled files..${NC}"
rm program

