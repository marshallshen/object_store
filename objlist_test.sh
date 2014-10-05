#!/bin/bash
green='\033[0;32m'
yellow='\033[0;33m'
NC='\e[0m'

echo -e "${yellow}[Before Test] Compile targeted files..${NC}"
make clean > logfile
make all > logfile

echo -e "${green}[Test RUNNING] objlist lists view-able files of current user${NC}"
su u1 -c "./objput x < testfile"
su u1 -c "./objput y < testfile"
su u1 -c "./objlist"
echo -e "${green}[Test OK] objlist lists view-able files of current user${NC}"

echo -e "${green}[Test RUNNING] objlist lists view-able files with file size when -l option is passed${NC}"
su u1 -c "./objlist -l"
echo -e "${green}[Test OK] objget lists view-able files with file size when -l option is passed${NC}"

echo -e "${green}[Test RUNNING] objlist view-able files of another user${NC}"
su u2 -c "./objlist -u u1"
echo -e "${green}[Test OK] objlist view-able files of another user${NC}"

echo -e "${green}[Test RUNNING] Gracefully exit if the current user does not exist${NC}"
su nonexistent_user -c "./objlist -u u1"
echo -e "${green}[Test OK] Gracefully exit if the current user does not exist${NC}"

echo -e "${green}[Test RUNNING] Gracefully exit if the requested user does not exist${NC}"
su u2 -c "./objlist -u nonexistent_user"
echo -e "${green}[Test OK] Gracefully exit if the requested user does not exist${NC}"

echo -e "${yellow}[After Test] Clean up compiled files..${NC}"
make clean > logfile
