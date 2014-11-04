#!/bin/bash
green='\033[0;32m'
yellow='\033[0;33m'
NC='\e[0m'

echo -e "${yellow}[Before Test] Compile targeted files with OpenSSL libraries..${NC}"
make clean > logfile
echo -e "${yellow}[OpenSSL Compile] compile OpenSSL libraries..${NC}"
gcc -w -std=c99 -Wall -g -o program objput.c -lssl -lcrypto -Wno-pointer-sign > logfile
echo -e "${green} [Objput Test] Writes a three-line ecrypted file to the directory${NC}"
su u1 -c "./program -k "passphrase" x < testfile"
echo -e "${green} [Objput Test] Writes a ecrypted file with longer line to the directory${NC}"
su u1 -c "./program -k "secondpassphrase" y < testfile2"
echo -e "${green} [Objput Test] Writes an empty ecrypted file to the directory${NC}"
su u1 -c "./program -k "thirdpassphrase" z < testfile3"
echo -e "${green} [Test OK] Writes an ecrypted file to the directory${NC}"
echo -e "${yellow}[After Test] Clean up compiled files..${NC}"
rm program
make clean > logfile

