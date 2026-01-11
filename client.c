#include "networking.h"
#include <sys/select.h>
#include <time.h>


struct message {
  char msg[1025];
  char username[33];
  char time[8];
};



int main() {
  struct message messages[100];
  int num_messages = 0;
  char username[33];

  printf("Enter your username:\n");
  fgets(username, sizeof(username), stdin);
  // ncurses logic for prompting and reading username

  int server_socket;
  fd_set read_fds;

  server_socket = client_tcp_handshake("127.0.0.1");

  


  while (1) {
    FD_ZERO(&read_fds);


    FD_SET(STDIN_FILENO, &read_fds);
    FD_SET(server_socket, &read_fds);

    printf("Enter message here:\n");
    char buf[1025];

    select(server_socket + 1, &read_fds, NULL, NULL, NULL);

    if (FD_ISSET(STDIN_FILENO, &read_fds)) {
      if (fgets(buf, sizeof(buf), stdin) == NULL) err(-1, "read message from stdin");;
      
      buf[sizeof(buf) - 1] = '\0';

      struct message new_msg;
      strcpy(new_msg.msg, buf);
      strcpy(new_msg.username, username);
      time_t raw_time;
      time(&raw_time);

      struct tm * local_time = localtime(&raw_time);

      char time[8];
      strftime(time, sizeof(time), "%I:%M%p", local_time);

      strcpy(new_msg.time, time);

      messages[num_messages] = new_msg;
      num_messages++;

      int bytes = write(server_socket, &new_msg, sizeof(new_msg));
      err(bytes, "write message to server");



    }
    if (FD_ISSET(server_socket, &read_fds)) {
      struct message msg;
      int bytes = read(server_socket, &msg, sizeof(msg));
      err(bytes, "read message from server");

      messages[num_messages] = msg;
      num_messages++;
    }


    // loop through all the messages, update ncurses
    printf("Current messages:\n");
    for (int i = 0; i < sizeof(messages) / sizeof(struct message); i++) {
      struct message m = messages[i];
      printf("%s", m.msg);
      
    }

  }



}





