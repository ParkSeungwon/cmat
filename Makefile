all : 
	make -C src/
	make -C tst/
	make -C matrix/
	make -C game/
	make -C obj/
	./catch.x

PHONY : clean

clean :
	rm obj/*.?
	rm *.x
