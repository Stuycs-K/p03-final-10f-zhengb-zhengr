#include "networking.h"
    #include <sys/select.h>
    #include <time.h>
    #include <ncurses.h>


    #define msg_MAX 100
    #define usr_MAX 50




    int main() {
      initscr();
      chbreak();
      noecho();
      keypad(stdscr,TRUE);//this is for keys liie the up arrow and down arror



      char messages[100][FULL_MSG_SIZE];
      int num_messages = 0;
      char username[33];

      mvprintw(0,0,"Enter your Username: ");
      echo();
      getnstr(username,32);//so they have ttheir actual username

      if(username[0] == "\0"){
        strcpy(username,"Guest");
      }

      clear();

     

      int server_socket = client_tcp_handshake("127.0.0.1");
      fd_set read_fds;

      while (1) {
        FD_ZERO(&read_fds);

        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(server_socket, &read_fds);

        printf("Enter message here:\n"); 
        char buf[1025];

        select(server_socket + 1, &read_fds, NULL, NULL, NULL);

        
        if (FD_ISSET(server_socket, &read_fds)) {
          char msg[FULL_MSG_SIZE];

          int bytes = read(server_socket, msg, sizeof(msg));
          err(bytes, "read message from server");
          msg[bytes] = '\0';

          strcpy(messages[num_messages], msg);
          num_messages++;

          // loop through all the messages, update ncurses
          printf("\nCurrent messages:\n");
          for (int i = 0; i < num_messages; i++) {
            printf("%s", messages[i]);
          }
          printf("\n"); 
        }

        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
          if (fgets(buf, sizeof(buf), stdin) == NULL) break;
          buf[strlen(buf) - 1] = '\0';


          char new_msg[FULL_MSG_SIZE];

          time_t raw_time;
          time(&raw_time);

          struct tm * local_time = localtime(&raw_time);

          char time[8];
          strftime(time, sizeof(time), "%I:%M%p", local_time);
          time[sizeof(time) - 1] = '\0';

          snprintf(new_msg, sizeof(new_msg), "%s: %s (%s)\n", username, buf, time);

          int bytes = write(server_socket, new_msg, strlen(new_msg));
          err(bytes, "write message to server");

          // printf("WRITE!\n");



        }

      }

      close(server_socket);



    }