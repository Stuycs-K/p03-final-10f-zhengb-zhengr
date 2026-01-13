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

      for (int i = 0; i < num_clients; i++) {
        FD_SET(client_fds[i], &read_fds);
      }
      int can_read = select(max_fd+1, &read_fds, NULL, NULL, NULL);
      err(can_read, "select error");

      if (FD_ISSET(listen_socket, &read_fds)) {
        int client_socket = server_tcp_handshake(listen_socket);
        FD_SET(client_socket, &read_fds);

        if (client_socket > max_fd) max_fd = client_socket;
        client_fds[num_clients++] = client_socket;
      }

      for (int i = 0; i < num_clients; i++) { // one issue with this is that earlier clients will have read priority
        // probably causes the issue of the later clients not having their messages displayed until the first client sends a message

        if (FD_ISSET(client_fds[i], &read_fds)) {
          // printf("SERVER READ\n");  

          char msg[MAX_MSG_SIZE];
          int client_socket = client_fds[i];
          int bytes = read(client_socket, msg, sizeof(msg));

          if (bytes == 0) {
            printf("client closed\n");
            close(client_socket);

            // remove client from client_fds
            for (int j = i; j < num_clients-1; j++) {
              client_fds[j] = client_fds[j+1];
            }
            num_clients--;
          }

          else {
            for (int j = 0; j < num_clients; j++) {
              write(client_fds[j], msg, bytes);
            }
          }


        }

      }



    }

    close(listen_socket);




}
