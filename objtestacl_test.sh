#!/bin/bash
green='\033[0;32m'
yellow='\033[0;33m'
NC='\e[0m'

echo -e "${yellow}[Before Test] Compile targeted files..${NC}"
make clean > logfile
make all > logfile

echo -e "${green}[Test RUNNING] objtestacl returns whether an action on a file is allowed for a given user${NC}"
su u1 -c "./objput x < testfile"
echo -e "${green}[Test SPEC] Expect return allowed${NC}"
su u1 -c "./objtestacl -x w u1+x"
echo -e "${green}[Test DENIED] Expect return allowed${NC}"
su u2 -c "./objtestacl -x w u1+x"
echo -e "${green}[Test OK] objtestacl returns whether an action on a file is allowed for a given user${NC}"

echo -e "${green}[Test RUNNING] objtestacl gracefully exists when a file does not exist${NC}"
su u1 -c "./objput x < testfile"
su u1 -c "./objtestacl -x w u1+y"
echo -e "${green}[Test OK] objtestacl gracefully exists when a file does not exist${NC}"

echo -e "${yellow}[After Test] Clean up compiled files..${NC}"
make clean > logfile
