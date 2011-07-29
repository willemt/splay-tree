CC     = gcc


all: tests

main.c:
	sh make-tests.sh > main.c

tests: main.c splaytree.o test_splaytree.c CuTest.c main.c
	$(CC) -o $@ $^
	./tests

splaytree.o: splaytree.c
	$(CC) -c -o $@ $^

clean:
	rm -f main.c splaytree.o tests
