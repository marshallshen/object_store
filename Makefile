FLAGS=-Wall -g -std=c99 -lssl -lcrypto -Wno-pointer-sign
export CC=c99

build:
	@echo "Adding user and group credentials.."

ifneq "$(strip $(userfile))" ""
exec: build
	@echo found $(userfile)
	@while read line; do echo $${line}; done < $(userfile)
else
exec: build
	@echo add default group setup
	@sudo groupadd g1
	@sudo groupadd g2
	@sudo groupadd g3
	@echo add default user setup
	@echo u1 g1 g2
	@sudo useradd -G g1,g2 u1
	@sudo passwd u1
	@echo u2 g2 g3
	@sudo useradd -G g2,g3 u2
	@sudo passwd u2
endif

all:
	make objget
	make objput
	make objlist
	make objsetacl
	make objgetacl
	make objtestacl
	chmod u+s objtestacl
clean:
	touch objput
	rm objput
	touch objget
	rm objget
	touch objlist
	rm objlist
	touch objsetacl
	rm objsetacl
	touch objgetacl
	rm objgetacl
	touch objtestacl
	rm objtestacl
	rm -rf acls
	mkdir acls
	chmod 777 acls
	rm -rf objects
	mkdir objects
	chmod 777 objects
	touch newacl
	rm newacl
	rm -rf keys
	mkdir keys
	chmod 777 keys

