.PHONY: compile clean server client
compile: client.o server.o networking.o
	@gcc -o client client.o networking.o
	@gcc -o server server.o networking.o

client.o: client.c networking.h
	@gcc -Wall -c client.c -lncurses

server.o: server.c networking.h
	@gcc -Wall -c server.c -lncurses

networking.o: networking.c networking.h
	@gcc -Wall -c networking.c
server:
	@./server
client:
	@./client
clean:
	@rm *.o server client
