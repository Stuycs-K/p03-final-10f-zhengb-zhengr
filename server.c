#include "networking.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

#define client_MAX 50

int main(int argc, char *argv[] ) {

  int listen_socket = server_setup();

  int client_sockets[client_MAX];
  int client_count = 0;

  fd_set read_fds;

  while(1 == 1){

    int max_fd;
    int ready;
    int i;

    FD_ZERO(&read_fds);

    FD_SET(listen_socket, &read_fds);

    max_fd = listen_socket;

    i = 0;
    while(i < client_count){
      FD_SET(client_sockets[i], &read_fds);

      if(client_sockets[i] > max_fd){
        max_fd = client_sockets[i];
      }

      i += 1;
    }

    ready = select(max_fd + 1, &read_fds, NULL, NULL, NULL);

    if(FD_ISSET(listen_socket, &read_fds)){
      int other_client;

      other_client = server_tcp_handshake(listen_socket);

      if(client_count < client_MAX){
        client_sockets[client_count] = other_client;
        client_count += 1;
        printf("new client connetd");
      }
      else{
        close(other_client);
      }
    }

    i = 0;
    while(i < client_count){
      int fd;
      int readyRead;

      fd = client_sockets[i];
      readyRead = FD_ISSET(fd, &read_fds);

      if(readyRead){
        char buffer[BUFFER_SIZE];
        int bytes_read;

        bytes_read = read(fd, buffer, BUFFER_SIZE - 1);

        if(bytes_read <= 0){

          printf("client disconnected");
          close(fd);

          {
            int temp = i;
            while(temp < client_count - 1){
              client_sockets[temp] = client_sockets[temp + 1];
              temp += 1;
            }
          }

          client_count -= 1;

        }
        else{
          buffer[bytes_read] = '\0';
          printf("recieved %s from client", buffer);

          i += 1;
        }
      }
      else{
        i += 1;
      }
    }
  }
  return 0;
}
