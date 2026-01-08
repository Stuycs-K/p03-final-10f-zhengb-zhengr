#include <curses.h>
#include "networking.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/smh.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define SHARED_MEM_KEY "13723"
int main() {
    char[1025] most_recent_message = "";
    int shmid;

    shmid = shmget(SHARED_MEM_KEY, sizeof(most_recent_message), IPC_CREAT | 0640);
    most_recent_message = shmat(shmid, 0, 0);


    int listen_socket; int client_socket;

    fd_set read_fds;
    listen_socket = server_setup();

    char buff[1025];

    while(1) {
      FD_ZERO(&read_fds);

      FD_SET(listen_socket, &read_fds);
      FD_SET(STDIN_FILENO, &read_fds);

      int i = select(listen_socket+1, &read_fds, NULL, NULL, NULL);

      if (FD_ISSET(STDIN_FILENO, &read_fds)) {
        int bytes = fgets(buff, sizeof(buff), stdin);
        err(bytes, "read from stdin");
        buff[strlen(buff)-1] = 0;
      }

      if (FD_ISSET(listen_socket, &read_fds)) {
        client_socket = server_tcp_handshake(listen_socket);

        int bytes = read(client_socket, buf, sizeof(buff));
        err(bytes, "read from cient");









      }
    }




}
