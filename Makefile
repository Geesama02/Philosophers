CFLAGS = -Wall -Wextra -Werror
CC = cc
NAME = philo
SRC = main.c ft_atoi.c parse.c
OBJS = ${SRC:.c=.o}

all : ${NAME}

${NAME} : ${OBJS}
	${CC} ${CFLAGS} -pthread ${OBJS} -o ${NAME}

%.o: %.c philo.h
	${CC} ${CFLAGS} -c -o $@ $<

clean : 
	rm -rf ${OBJS}

fclean : clean
	rm -rf ${NAME}

re : fclean all

.PHONY : all clean fclean re bonus