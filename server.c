#include <curses.h>
#include "networking.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>

int main() {
    int listen_socket; int client_socket;
    
    fd_set read_fds;
    listen_socket = server_setup();
    
    FD_ZERO(&read_fds);

    FD_SET(listen_socket, &read_fds);
    FD_SET(STDIN_FILENO, &read_fds);

    int i = select(listen_socket+1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
        
    }

    if (FD_ISSET(listen_socket, &read_fds)) {
        client_socket = server_tcp_handshake(listen_socket)
    }


}