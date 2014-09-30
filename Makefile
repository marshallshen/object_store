FLAGS=-Wall -g -std=c99
export CC=c99

build:
	make objget
	make objput
	make objlist
	make objsetacl
	make objgetacl
	make objtestacl
clean:
	rm objget
	rm objgetacl
	rm objput
	rm objsetacl
	rm objtestacl
	rm Accessfile
	rm -rf objects/
	touch Accessfile
	mkdir objects
test1:
	./objput -u u1 -g g1 doc < testfile
	./objget -u u1 -g g1 doc
	./objgetacl -u u1 -g g1 doc
	./objtestacl -u u1 -g g3 -a r doc
	./objtestacl -u u1 -g g3 -a x doc
test2:
	./objput -u marshall -g g1 doc < testfile
	./objtestacl -u marshall -g g1 -a r doc
	touch newacl
	echo "marshall.* r" > newacl
	./objsetacl -u marshall -g g1 doc < newacl
	rm newacl
	./objtestacl -u marshall -g g1 -a x doc
