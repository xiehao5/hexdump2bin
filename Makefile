all:main.c
	gcc -o hexdump2bin.exe -Wall -g3 main.c

clean:
	rm hexdump2bin.exe