#include <curses.h>
#include "networking.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>

int main() {
    int listen_socket;
    listen_socket = server_setup();


    fd_set read_fds;
    int client_fds[10];
    int num_clients = 0;
    int max_fd = listen_socket;



    while(1) {
      FD_ZERO(&read_fds);
      FD_SET(listen_socket, &read_fds);
     
      int can_read = select(max_fd+1, &read_fds, NULL, NULL, NULL);
      err(can_read, "select error");

      if (FD_ISSET(listen_socket, &read_fds)) {
        int client_socket = server_tcp_handshake(listen_socket);
        FD_SET(client_socket, &read_fds);

        max_fd = max(max_fd, client_socket);
        client_fds[num_clients++] = client_socket;
      }

      for (int i = 0; i < num_clients; i++) {
        if (FD_ISSET(client_fds[i], &read_fds)) {
          char buf[1025];
          int client_socket = client_fds[i];
          int bytes = read(client_socket, buf, sizeof(buf));
          buf[sizeof(buf) - 1] = '\0';

          if (bytes == 0) {
            // close client socket
            // remove client from client_fds
          }

          else {
            for (int j = 0; j < num_clients; j++) {
              if (j != i) write(client_fds[j], buf, bytes);
            }
          }


        }

      }

      
      
    }




}
