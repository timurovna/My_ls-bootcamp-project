CC=gcc
CC_OPT=-g3 -fsanitize=address -Wall -Wextra -Werror
OUTPUT=myls

${OUTPUT}:
	${CC} ${CC_OPT} -o ${OUTPUT} main.c myls.c -I.

clean:
	rm ${OUTPUT}