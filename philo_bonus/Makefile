CFLAGS = -Wall -Wextra -Werror
CC = cc
NAME = philo_bonus
SRC = main_bonus.c parse_bonus.c ft_atoi_bonus.c routine_bonus.c monitor_bonus.c sleep_utils_bonus.c utils_bonus.c init_bonus.c
OBJS = ${SRC:.c=.o}

all : ${NAME}

${NAME} : ${OBJS}
	${CC} ${CFLAGS} -pthread ${OBJS} -o ${NAME}

%.o: %.c philo_bonus.h
	${CC} ${CFLAGS} -c -o $@ $<

clean : 
	rm -rf ${OBJS}

fclean : clean
	rm -rf ${NAME}

re : fclean all

.PHONY : all clean fclean re