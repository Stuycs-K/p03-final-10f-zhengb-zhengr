#include "networking.h"
#include <sys/select.h>

struct message {
  char msg[1025];
  char username[33];
  char time[9];
};


int main() {
  struct message messages[100];

  int server_socket;
  fd_set read_fds;

  server_socket = client_tcp_handshake("127.0.0.1");


  while (1) {
    FD_ZERO(&read_fds);

    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(server_socket, &read_fds);

    select(server_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      char buf[1025];

      int bytes = fgets(buf, sizeof(buf), stdin);
      err(bytes, "read from stdin");

      bytes = write(server_socket, buf, sizeof(buf));
      err(bytes, "write message to server");



    }
    if (FD_ISSET(server_socket, &read_fds)) {
      char buf[1025];


    }

  }



}





