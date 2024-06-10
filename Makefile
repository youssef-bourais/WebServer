# NAME = webserver
#
# SRCS = main.cpp HttpServer.cpp HttpMessage.cpp HttpRequest.cpp HttpResponse.cpp Tools.cpp
#
# CC = c++ -std=c++98 -g -fsanitize=address
#
# CFLAGS = -Wall -Werror -Wextra
#
# OBJS = ${SRCS:.cpp=.o}
#
# %.o : %.cpp	
# 		$(CC) $(CFLAGS) -c $<
#
# all : $(NAME)
#
# $(NAME) : $(OBJS) HttpMessage.hpp HttpRequest.hpp HttpResponse.hpp HttpServer.hpp Tools.hpp Makefile
# 		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
# clean :
# 	rm -rf $(OBJS)
#
# fclean : clean
# 	rm -rf $(NAME)
#
# re : fclean all


NAME = webserver

SRCS = main.cpp HttpServer.cpp HttpMessage.cpp HttpRequest.cpp HttpResponse.cpp Tools.cpp

# SRC = main.cpp ./server/Server.cpp ./server/serv.cpp

CC = c++

CPPFLAGS = -std=c++98 -fsanitize=address

ERROR_FILES = ../SalahServer2/errors/Errors.cpp

ERROR_HEADERS = ../SalahServer2/errors/Errors.hpp

PARSING_FILES = ../SalahServer2/parsing/Parsing.cpp ../SalahServer2/parsing/FileReader.cpp ../SalahServer2/parsing/parse.cpp ../SalahServer2/parsing/parsingFuncs.cpp ../SalahServer2/parsing/errors.cpp

PARSING_HEADER = ../SalahServer2/parsing/Parsing.hpp ../SalahServer2/parsing/FileReader.hpp ../SalahServer2/parsing/parse.hpp ../SalahServer2/parsing/parsingStruct.hpp


PARSING_OBJS = ${PARSING_FILES:.cpp=.o}

SRC_OBJS = ${SRCS:.cpp=.o}

ERROR_OBJS = ${ERROR_FILES:.cpp=.o}

%.o: %.cpp ${PARSING_HEADER} ${ERROR_HEADERS}  HttpMessage.hpp HttpRequest.hpp HttpResponse.hpp HttpServer.hpp Tools.hpp  
	${CC} ${CPPFLAGS} -c $< -o $@

${NAME}: ${SRC_OBJS} ${ERROR_OBJS} ${ERROR_HEADERS} ${PARSING_OBJS} ${PARSING_HEADER}  HttpMessage.hpp HttpRequest.hpp HttpResponse.hpp HttpServer.hpp Tools.hpp 
	${CC} ${CPPFLAGS} ${SRC_OBJS} ${ERROR_OBJS} ${PARSING_OBJS} -o ${NAME}

all: ${NAME}

clean:
	rm -fr ${SRC_OBJS} ${ERROR_OBJS} ${PARSING_OBJS}

fclean: clean
	rm -fr ${NAME}

re: fclean all

