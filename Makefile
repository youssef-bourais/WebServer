NAME = webserver

SRCS = main.cpp

CC = c++ 

CFLAGS = -Wall -Werror -Wextra

OBJS = ${SRCS:.cpp=.o}

%.o : %.cpp	
		$(CC) $(CFLAGS) -c $<

all : $(NAME)

$(NAME) : $(OBJS) Makefile
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all
