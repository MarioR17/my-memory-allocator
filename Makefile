all:
	gcc -g ./src/main.c ./src/heap.c -o ./main -Wall -Wextra -Werror -pedantic -fno-builtin

clean:
	rm ./main
