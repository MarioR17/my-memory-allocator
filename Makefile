all:
	gcc ./src/main.c -o ./main -Wall -Wextra -Werror -pedantic

clean:
	rm ./main
