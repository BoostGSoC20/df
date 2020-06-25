CC=g++
# CC=clang++
CFLAGS=-std=c++17 -I ../../include/

run: column
	./column

column:
	$(CC) $(CFLAGS) -o ./column ./column.cpp

clean:
	rm ./column

.PHONY: run clean
