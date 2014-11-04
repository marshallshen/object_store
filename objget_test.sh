#!/bin/bash
green='\033[0;32m'
yellow='\033[0;33m'
NC='\e[0m'

echo -e "${yellow}[Before Test] Compile targeted files with OpenSSL libraries..${NC}"
make clean > logfile
echo -e "${yellow}[Objput TEST] Compile objput${NC}"
gcc -w -std=c99 -Wall -g -o putprogram objput.c -lssl -lcrypto -Wno-pointer-sign > logfile
echo -e "${green} [Objget Test] Writes an ecrypted file to the directory${NC}"
echo -e "${green} [Objget Test] Create file u1+x..  ${NC}"
su u1 -c "./putprogram -k "passphrase" x < testfile"
echo -e "${green} [Objget Test] Create file u1+y..  ${NC}"
su u1 -c "./putprogram -k "whataboutbob" y < testfile2"
echo -e "${green} [Objget Test] key file ${NC}"
echo -e "${green} [Objget Test] Create file u1+y..  ${NC}"
su u1 -c "./putprogram -k "lordofrings" z < testfile3"

echo -e "${yellow} [Objget TEST] Compile objget..${NC}"
gcc -w -std=c99 -Wall -g -o getprogram objget.c -lssl -lcrypto -Wno-pointer-sign > logfile
echo -e "${green} [Objget Test] Display decrypted u1+x...${NC}"
su u1 -c "./getprogram -k "passphrase" u1+x"
echo -e "${green} [Objget Test] Display decrypted u1+y...${NC}"
su u1 -c "./getprogram -k "whataboutbob" u1+y"
echo -e "${green} [Objget Test] Display decrypted u1+z...${NC}"
su u1 -c "./getprogram -k "lordofrings" u1+z"
echo -e "${green} [Test OK] Writes an ecrypted file to the directory${NC}"

echo -e "${green} [Objget Test] Rerturns error is passphrase is invalid..${NC}"
su u1 -c "./getprogram -k "invalidpassphrase" u1+x"
echo -e "${green} [Test OK] Rerturns error is passphrase is invalid..${NC}"

echo -e "${green} [Objget Test] Returns permission denied without decryption if user is not allowed to read the file${NC}"
su u2 -c "./getprogram -k "passphrase" u1+x"
echo -e "${green} [Test OK] Writes an ecrypted file to the directory${NC}"

echo -e "${yellow}[After Test] Clean up compiled files..${NC}"
rm putprogram > logfile
rm getprogram > logfile
make clean > logfile
