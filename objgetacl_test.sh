#!/bin/bash
green='\033[0;32m'
yellow='\033[0;33m'
NC='\e[0m'

echo -e "${yellow}[Before Test] Compile targeted files..${NC}"
make clean > logfile
make all > logfile

echo -e "${green}[Test RUNNING] objgetacl finds permission of a file of the current user${NC}"
su u1 -c "./objput x < testfile"
su u1 -c "./objgetacl u1+x"
echo -e "${green}[Test OK] objgetacl finds permission a file of the current user"

echo -e "${green}[Test RUNNING] objgetacl finds permission of a file of another user, if permission is allowed${NC}"
su u2 -c "./objput y < testfile"
su u1 -c "./objgetacl u2+y"
echo -e "${green}[Test OK] objget finds a file of another user${NC}"

echo -e "${green}[Test RUNNING] objgetacl returns gracefully if a file does not exist${NC}"
su u1 -c "./objgetacl u1+an-nonexistent-file"
echo -e "${green}[Test OK] objgetacl returns gracefully if a file does not exist${NC}"

echo -e "${yellow}[After Test] Clean up compiled files..${NC}"
make clean > logfile
