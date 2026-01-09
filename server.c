#include "networking.h"



int main(int argc, char *argv[] ) {
  int listen_socket = server_setup();

  while(1 == 1){
    int client_socket = server_tcp_handshake(listen_socket);
    int listen_socket = server_setup();
    int client_count = 0; //0 clients at the start
    printf("new client connected to server \n");

    while(1 == 1){
        int max_fd;
        int ready;
        int i;

        FD_ZERO(&read_fds);

        FD_SET(listen_socket,&read_fds);

        max_fd = listen_socket;
        i = 0;
        while(i < client_count){
            FD_SET(client_sockets[i],&read_fds);

            if(client_sockets[i] > max_fd){
                max_fd =client_sockets[i];
            }
        }
        i += 1;
    }

    ready = select(max_fd + 1,&read_fds,NULL,NULL,NULL);

    if(FD_ISSET(listen_socket,&read_fds)){
        int other_client;

        other_client = server_tcp_handshake(listen_socket);

        if(client_count < MAX)
    }



  

  }

}