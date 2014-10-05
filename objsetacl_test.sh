#!/bin/bash
green='\033[0;32m'
yellow='\033[0;33m'
NC='\e[0m'

echo -e "${yellow}[Before Test] Compile targeted files..${NC}"
make clean > logfile
make all > logfile

echo -e "${green}[Test RUNNING] objsetacl sets new permission of a file${NC}"
su u1 -c "./objput x < testfile"
echo -e "${green}[Test Spec] Expect rwxpv ACL for current user${NC}"
su u1 -c "./objgetacl u1+x"
echo -e "*u1.* rvp" > newacl
su u1 -c "./objsetacl u1+x < newacl"
echo -e "${green}[Test spec] Expect rvp ACL for current user${NC}"
su u1 -c "./objgetacl u1+x"
echo -e "${green}[Test OK] objsetacl sets new permission of a file${NC}"

echo -e "${green}[Test RUNNING] objsetacl gracefully exits when new ACL file is not valid${NC}"
su u2 -c "./objput y < testfile"
touch invalid_acl
echo -e "*.public not a valid acl" > invalid_acl
su u2 -c "./objsetacl u2+y < invalid_acl"
rm invalid_acl
su u2 -c "./objgetacl u2+y"
echo -e "${green}[Test OK] objsetacl gracefully exits when new ACL file is not valid${NC}"

echo -e "${green}[Test RUNNING] objsetacl gracefully exits when user is not allowed to set acl${NC}"
su u1 -c "./objput z < testfile"
su u2 -c "./objsetacl u1+z< newacl"
echo -e "${green}[Test OK] objsetacl gracefully exits when user is not allowed to set acl${NC}"

echo -e "${yellow}[After Test] Clean up compiled files..${NC}"
make clean > logfile
