#include "networking.h"

void subserver_logic(int client_socket){
  char buffer [BUFFER_SIZE];

  read(client_socket,buffer,sizeof(buffer));

  for(int i = 0; buffer[i] != '\0';i++){
    if(buffer[i]>= 'a' && buffer[i] <= 'z'){
      buffer[i] = ((buffer[i] - 'a' + 13) % 26) + 'a';
    }
    else if(buffer[i] >= 'A' && buffer[i] <= 'Z'){
      buffer[i] = (buffer[i] -  'A' + 13) % 26 + 'A';
    }
  }

  write(client_socket,buffer,strlen(buffer)+ 1);
  close(client_socket);
}

int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();

  while(1 == 1){
    int client_socket = server_tcp_handshake(listen_socket);
    int listen_socket;
    int client_count = 0; //0 clients at the start
    printf("new client connected to server \n");



  if(fork() == 0){
    close(listen_socket);
    subserver_logic(client_socket);
    exit(0);
  }

  }

}