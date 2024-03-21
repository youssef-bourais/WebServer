SRC = server.cpp client.cpp

OBJ = ${SRC:.cpp=.o}

CFLAGS = -Wall -Wextra -Werror

all:server client

client:$(OBJ)
	c++ $(CFLAGS) client.o -o client 
server:$(OBJ)
	c++ $(CFLAGS) server.o -o server

%.o:%.cpp
	c++ $(CFLAGS) $^ -c

clean:
	rm -f $(OBJ)

fclean:clean
	rm -f server client

re:fclean server client
