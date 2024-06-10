NAME = webserver

SRCS = main.cpp

CC = c++

CPPFLAGS = -std=c++98 -g -fsanitize=address


#Salah

ERROR_FILES = ./errors/Errors.cpp

ERROR_HEADERS = ./errors/Errors.hpp

PARSING_FILES = ./parsing/Parsing.cpp ./parsing/FileReader.cpp ./parsing/parse.cpp ./parsing/parsingFuncs.cpp ./parsing/errors.cpp

PARSING_HEADER = ./parsing/Parsing.hpp ./parsing/FileReader.hpp ./parsing/parse.hpp ./parsing/parsingStruct.hpp

#Youssef

ResponseFiles = ./Response/HttpResponse.cpp 

RequestFiles = ./Request/HttpRequest.cpp ./Request/HttpMessage.cpp

ServerFiles = ./Server/HttpServer.cpp 

Tools = ./Tools/Tools.cpp

Headers = ./Request/HttpMessage.hpp ./Request/HttpRequest.hpp ./Response/HttpResponse.hpp ./Server/HttpServer.hpp ./Tools/Tools.hpp


SRC_OBJS = ${SRCS:.cpp=.o}

ERROR_OBJS = ${ERROR_FILES:.cpp=.o}

PARSING_OBJS = ${PARSING_FILES:.cpp=.o}

RESP_OBJS = ${ResponseFiles:.cpp=.o}

REQ_OBJS = ${RequestFiles:.cpp=.o}

SERV_OBJS = ${ServerFiles:.cpp=.o}

TOOLS_OBJS = ${Tools:.cpp=.o}

%.o: %.cpp  
	$(CC) $(CPPFLAGS) -c $<

${NAME}: ${SRC_OBJS} ${ERROR_OBJS} ${PARSING_OBJS} ${REQ_OBJS} ${RESP_OBJS} ${SERV_OBJS} ${TOOLS_OBJS} ${PARSING_HEADER} ${ERROR_HEADERS} ${Headers} Makefile
	${CC} ${CPPFLAGS} ${SRC_OBJS} ${ERROR_OBJS} ${PARSING_OBJS} -o ${NAME}

all: ${NAME}

clean:
	rm -fr ${SRC_OBJS} ${ERROR_OBJS} ${PARSING_OBJS}

fclean: clean
	rm -fr ${NAME}

re: fclean all

