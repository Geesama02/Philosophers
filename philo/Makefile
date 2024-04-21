CFLAGS = -Wall -Wextra -Werror
CC = cc
NAME = philo
SRC = main.c ft_atoi.c parse.c init_utils.c clean_utils.c monitor.c routine.c \
		sleep_utils.c threads_utils.c utils.c
OBJS = ${SRC:.c=.o}

all : ${NAME}

${NAME} : ${OBJS}
	${CC} ${CFLAGS} -g -fsanitize=thread -pthread ${OBJS} -o ${NAME}

%.o: %.c philo.h
	${CC} ${CFLAGS} -g -fsanitize=thread -c -o $@ $<

clean : 
	rm -rf ${OBJS}

fclean : clean
	rm -rf ${NAME}

re : fclean all

.PHONY : all clean fclean re