compile:
	gcc -Isrc -std=c11 src/main.c -o file_system

format:
	cd src && clang-format -i -style=google *.h *.c

clean:
	rm file_system

