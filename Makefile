
NAME = webserver


# Source files
SRCS = main.cpp
ERROR_FILES = ./errors/Errors.cpp
PARSING_FILES = ./parsing/Parsing.cpp ./parsing/FileReader.cpp ./parsing/parse.cpp ./parsing/parsingFuncs.cpp ./parsing/errors.cpp
RESPONSE_FILES = ./Response/HttpResponse.cpp
REQUEST_FILES = ./Request/HttpRequest.cpp ./Request/HttpMessage.cpp ./Request/RequestParsser.cpp
SERVER_FILES = ./Server/HttpServer.cpp
TOOLS_FILES = ./Tools/Tools.cpp

# Header files
ERROR_HEADERS = ./errors/Errors.hpp
PARSING_HEADERS = ./parsing/Parsing.hpp ./parsing/FileReader.hpp ./parsing/parse.hpp ./parsing/parsingStruct.hpp
HEADERS = ./Request/HttpMessage.hpp ./Request/HttpRequest.hpp ./Response/HttpResponse.hpp ./Server/HttpServer.hpp ./Tools/Tools.hpp ./Request/RequestParsser.hpp

# Compiler and flags
CC = c++ 
CPPFLAGS = -Wall -Werror -Wextra -std=c++98 -g -fsanitize=address 

# Object files
SRC_OBJS = $(SRCS:.cpp=.o)
ERROR_OBJS = $(ERROR_FILES:.cpp=.o)
PARSING_OBJS = $(PARSING_FILES:.cpp=.o)
RESP_OBJS = $(RESPONSE_FILES:.cpp=.o)
REQ_OBJS = $(REQUEST_FILES:.cpp=.o)
SERV_OBJS = $(SERVER_FILES:.cpp=.o)
TOOLS_OBJS = $(TOOLS_FILES:.cpp=.o)

# Rule to build object files from source files
%.o: %.cpp  
	$(CC) $(CPPFLAGS) -c $< -o $@

# Rule to build the final executable
$(NAME): $(SRC_OBJS) $(ERROR_OBJS) $(PARSING_OBJS) $(REQ_OBJS) $(RESP_OBJS) $(SERV_OBJS) $(TOOLS_OBJS) ${ERROR_HEADERS} ${PARSING_HEADERS} ${HEADERS} Makefile
	$(CC) $(CPPFLAGS) -o $(NAME) $(SRC_OBJS) $(ERROR_OBJS) $(PARSING_OBJS) $(REQ_OBJS) $(RESP_OBJS) $(SERV_OBJS) $(TOOLS_OBJS)

all: $(NAME)

clean:
	rm -f $(SRC_OBJS) $(ERROR_OBJS) $(PARSING_OBJS) $(REQ_OBJS) $(RESP_OBJS) $(SERV_OBJS) $(TOOLS_OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
