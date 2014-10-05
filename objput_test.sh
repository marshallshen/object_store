#!/bin/bash
green='\033[0;32m'
yellow='\033[0;33m'
NC='\e[0m'

echo -e "${yellow}[Before Test] Compile targeted files..${NC}"
make clean > logfile
make all > logfile

echo -e "${green}[Test RUNNING] objput creates a file for a given user${NC}"
su u1 -c "./objput x < testfile"
echo -e "${green}[Test OK] objput creates a file for a given user${NC}"

echo -e "${yellow}[After Test] Clean up compiled files..${NC}"
make clean > logfile
