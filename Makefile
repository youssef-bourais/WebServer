NAME = webserver

SRCS = main.cpp HttpServer.cpp HttpMessage.cpp HttpRequest.cpp HttpResponse.cpp

CC = c++ -std=c++98

CFLAGS = -Wall -Werror -Wextra

OBJS = ${SRCS:.cpp=.o}

%.o : %.cpp	
		$(CC) $(CFLAGS) -c $<

all : $(NAME)

$(NAME) : $(OBJS) HttpMessage.hpp HttpRequest.hpp HttpResponse.hpp HttpServer.hpp Makefile
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all
