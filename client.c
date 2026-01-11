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

      mvwprintw(0,0,"Enter your Username: ");
      echo();
      getnstr(username,32);//so they have ttheir actual username

      if(username[0] == "\0"){
        strcpy(username,"Guest");
      }

      clear();


      int server_socket = client_tcp_handshake("127.0.0.1");

      int screen_h;
      int screen_w;

      getmaxyx(stdscr,screen_h,screen_w);

      int input_h = 3;
      int top_h = screen_h - input_h;//rest of the space goes to chat + userlist


      int users_W = 20;
      int chat_w = screen_w - users_w;//rest of this space goes to chat

      WINDOW * users_WIN = newwin(top_h,users_w,0,0);

      WINDOW * chat_WIN = newwin(top_h,chat_w,0,users_w);

      WINDOW * input_WIN = newwin(input_h,screen_w,top_h,0);

      char user_LIST[50][33];

      char input[1024];
      int input_LEN = 0;
      int scroll_CHAT = 0;
      int scroll_USER = 0;

      intput[0] = "\0";


      werase(users_WIN);
      box(users_WIN,0,0);
      mvwprintw(users_WIN,0,2,"Users ");
      wrefresh(users_WIN);

      werase(chat_WIN);
      box(chat_WIN,0,0);
      mvwprintw(chat_WIN,0,2,"Chat ");
      wrefresh(chat_WIN);

      werase(input_WIN);
      box(input_WIN,0,0);
      mvwprintw(input_WIN,0,2,"Input ");
      wrefresh(input_WIN);



      while (1) {
        fd_set read_fds;
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
            
          if(bytes > 0){
            buf[bytes] = "\0";
          

          if(num_messages < msg_MAX){
            strcpy(messages[num_messages],buf);
            num_messages++;
          }

          werase(chat_WIN);
          box(chat_WIN,0,0);
          mvwprintw(chat_WIN,0,2,"Chat ");
          // loop through all the messages, update ncurses
         
          for(int i = 0; i < num_messages && i < top_h - 2; i ++){
            mvwprintw(chat_WIN,i + 1, 1,messages[i]);
          }
          wrefresh(chat_WIN);
        }
    }



        if (FD_ISSET(STDIN_FILENO, &read_fds)) {

          int key = getch();

          if(key == KEY_BACKSPACE || key = 127){
            if(input_LEN > 0){
                input_LEN--;
                input[input_LEN] = "\0";
            }
          }

          else if(key == "\n"){
            if(input_LEN > 0){
                char new_msg[FULL_MSG_SIZE];
                

                time_t raw_time;
                time(&raw_time);

                struct tm * local_time = localtime(&raw_time);

                char time[8];
                strftime(time, sizeof(time), "%I:%M%p", local_time);

                snprintf(new_msg, sizeof(new_msg), "%s: %s (%s)\n", username, buf, time);

                int bytes = write(server_socket, new_msg, strlen(new_msg));
                err(bytes, "write message to server");

                input_LEN = 0;
                input[0] = "\0";
            }
          }
          
          else if(key >= 32 && key <= 126){//if its a n actual letter add to end
            if(input_LEN < 1024){
                input[input_LEN] = char(key);
                input_LEN ++;
                input[input_LEN] = "\0";
            }
          }

          werase(input_WIN);
          box(input_WIN,0,0);
          mvwprintw(input_WIN,0,2,"Input ");
          mvwprintw(input_WIN,1,1,"| %s",input);
          wrefresh(input_WIN);


        }

      }

      close(server_socket);
      endwin();
      return 0;



    }