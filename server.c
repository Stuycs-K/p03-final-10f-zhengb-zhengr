#include <curses.h>
#include "networking.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>


int listen_socket;
int num_clients = 0;
int client_fds[10];


static void sighandler(int signo) {
  if (signo == SIGINT) {
    close(listen_socket);
    for (int i = 0; i < num_clients; i++) {
      close(client_fds[i]);
    }
    exit(0);  
  }
}


int main() {
    signal(SIGINT, sighandler);

    listen_socket = server_setup();


    fd_set read_fds;
    int max_fd = listen_socket;

    char user_LIST[10][33];
    int has_user[10];

    for (int i = 0; i < 10; i++) {
      has_user[i] = 0;
      user_LIST[i][0] = '\0';
    }

    int first = 1;

    while(1) {
      FD_ZERO(&read_fds);
      FD_SET(listen_socket, &read_fds);

      max_fd = listen_socket;

      for (int i = 0; i < num_clients; i++) {
        FD_SET(client_fds[i], &read_fds);
        if (client_fds[i] > max_fd) max_fd = client_fds[i];
      }
      int can_read = select(max_fd+1, &read_fds, NULL, NULL, NULL);
      err(can_read, "select error");

      if (FD_ISSET(listen_socket, &read_fds)) {
        int client_socket = server_tcp_handshake(listen_socket);
        FD_SET(client_socket, &read_fds);

        if (client_socket > max_fd) max_fd = client_socket;
        client_fds[num_clients] = client_socket;
        has_user[num_clients] = 0;
        user_LIST[num_clients][0] = '\0';
        num_clients++;
      }

      for (int i = 0; i < num_clients; i++) { // one issue with this is that earlier clients will have read priority
        // probably causes the issue of the later clients not having their messages displayed until the first client sends a message
        if (FD_ISSET(client_fds[i], &read_fds)) {
          if (first) {
            first = !first;
            continue;
          }

          // printf("SERVER READ\n");

          char msg[MAX_MSG_SIZE];
          int client_socket = client_fds[i];

          int bytes = read(client_socket, msg, sizeof(msg));



          // printf("%d\n", bytes);

          if (bytes == 0) {
            printf("client closed\n");

            if (has_user[i] == 1) {
              char leave_msg[MAX_MSG_SIZE];
              snprintf(leave_msg, sizeof(leave_msg), "__LEAVE__:%s\n", user_LIST[i]);

              for (int j = 0; j < num_clients; j++) {
                if (j != i) {
                  write(client_fds[j], leave_msg, strlen(leave_msg));
                }
              }
            }

            close(client_socket);

            for (int j = i; j < num_clients-1; j++) {//removes client from the list
              client_fds[j] = client_fds[j+1];
              strcpy(user_LIST[j], user_LIST[j+1]);
              has_user[j] = has_user[j+1];
            }
            num_clients--;
            i--;
          }

          else {
            msg[bytes] = '\0';

            if (strncmp(msg, "__JOIN__:", 9) == 0) {
              char temp[33];
              int k = 0;

              while (k < 32 && msg[9 + k] != '\0' && msg[9 + k] != '\n') {
                temp[k] = msg[9 + k];
                k++;
              }
              temp[k] = '\0';

              strcpy(user_LIST[i], temp);
              has_user[i] = 1;

              for (int j = 0; j < num_clients; j++) {
                if (has_user[j] == 1) {
                  char join_msg[MAX_MSG_SIZE];
                  snprintf(join_msg, sizeof(join_msg), "__JOIN__:%s\n", user_LIST[j]);
                  write(client_fds[i], join_msg, strlen(join_msg));
                }
              }

              for (int j = 0; j < num_clients; j++) {
                write(client_fds[j], msg, strlen(msg));
              }
            }

            else {
              for (int j = 0; j < num_clients; j++) {
                write(client_fds[j], msg, bytes);
              }
            }
          }


        }

      }



    }

    close(listen_socket);




}