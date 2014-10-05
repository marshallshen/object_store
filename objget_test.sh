#!/bin/bash
green='\033[0;32m'
yellow='\033[0;33m'
NC='\e[0m'

echo -e "${yellow}[Before Test] Compile targeted files..${NC}"
make clean > logfile
make all > logfile

echo -e "${green}[Test RUNNING] objget finds a file of the current user${NC}"
su u1 -c "./objput x < testfile"
su u1 -c "./objget u1+x"
echo -e "${green}[Test OK] objget finds a file of the current user${NC}"

echo -e "${green}[Test RUNNING] objget finds a file of another user${NC}"
su u2 -c "./objput y < testfile"
su u1 -c "./objget u2+y"
echo -e "${green}[Test OK] objget finds a file of another user${NC}"

echo -e "${green}[Test RUNNING] objget returns gracefully if a file does not exist${NC}"
su u1 -c "./objget u1+an-nonexistent-file"
echo -e "${green}[Test OK] objget returns gracefully if a file does not exist${NC}"

echo -e "${yellow}[After Test] Clean up compiled files..${NC}"
make clean > logfile
