.PHONY: compile clean server client main

# compile: server client

# server: server.o networking.o
# 	@gcc -o server server.o networking.o

# client: client.o networking.o
# 	@gcc -o client client.o networking.o -lncurses

# server.o: server.c networking.h
# 	@gcc -Wall -c server.c

# client.o: client.c networking.h
# 	@gcc -Wall -c client.c

# networking.o: networking.c networking.h
# 	@gcc -Wall -c networking.c

main: main.c
	@gcc -Wall main.c -lncurses -o main

clean:
	rm -f *.o
	rm -f server client

