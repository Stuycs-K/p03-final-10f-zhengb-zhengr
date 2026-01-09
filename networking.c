#include "networking.h"



/*Create and bind a socket.
* Place the socket in a listening state.
* returns the socket descriptor
*/
int server_setup() {
  //setup structs for getaddrinfo
  struct addrinfo * hints;
  struct addrinfo * results;
  hints = (struct addrinfo *) malloc(1*sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, PORT, hints, &results);

  //create the socket
  int clientd;//store the socket descriptor here
  clientd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  //this code should get around the address in use error
  int yes = 1;
  int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  err(sockOpt,"sockopt  error");

  //bind the socket to address and port
  if (bind(clientd, results->ai_addr, results->ai_addrlen) < 0) perror("bind");
  //set socket to listen state
  listen(clientd, 10);


  //free the structs used by getaddrinfo
  free(hints);
  freeaddrinfo(results);

  return clientd;
}

/*Accept a connection from a client
 *return the socket descriptor for the new socket connected to the client
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    int client_socket;

    //accept() the client connection
    struct sockaddr_in incoming_addr;
    socklen_t addr_size = sizeof(incoming_addr);
    client_socket = accept(listen_socket, (struct sockaddr *)&incoming_addr, &addr_size);

    if (client_socket < 0) {
      perror("accpet client");
      exit(1);
    }

    return client_socket;
}


/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {

  //getaddrinfo
  struct addrinfo * hints, * results;
  hints = (struct addrinfo *) malloc(1*sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  getaddrinfo(server_address, PORT, hints, &results);

  int serverd;//store the socket descriptor here
  //create the socket
  serverd = socket(results->ai_family, results->ai_socktype, 0);
  err(serverd, "create socket");

  //connect() to the server
  int cnnt = connect(serverd, results->ai_addr, results->ai_addrlen);
  err(cnnt, "connect()");

  free(hints);
  freeaddrinfo(results);

  return serverd;
}




void err(int i, char*message){
  if(i < 0){
	  printf("Error: %s - %s\n",message, strerror(errno));
  	exit(1);
  }
}
