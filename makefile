.PHONY: compile clean server client main

compile: server.o client.o networking.o
	@gcc -o server server.o networking.o
	@gcc -o client client.o networking.o -lncurses

server:
	@./server

client:
	@./client $(ARGS)

server.o: server.c networking.h
	@gcc -Wall -c server.c

client.o: client.c networking.h
	@gcc -Wall -c client.c

networking.o: networking.c networking.h
	@gcc -Wall -c networking.c

main: main.c
	@gcc -Wall main.c -lncurses -o main

networking.o: networking.c networking.h
	@gcc -Wall -c networking.c
server:
	@./server
client:
	@./client
clean:
	@rm -f *.o
	@rm -f server client
