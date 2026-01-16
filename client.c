#include <sys/select.h>
#include <time.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "networking.h"

int server_socket;

static void sighandler(int signo) {
  if (signo == SIGINT) {
    close(server_socket);
    endwin();
    exit(0);
  }
}

int main(int argc, char * argv[]) {
  char * IP = "127.0.0.1";

  if (argc == 2) {
    IP = argv[1];
  }

  signal(SIGINT, sighandler);



  initscr();
  cbreak();
  noecho();
  keypad(stdscr,TRUE);//this is for keys liie the up arrow and down arror


  char messages[MAX_MSGS][MAX_MSG_SIZE];
  int num_messages = 0;
  char username[33];


  mvprintw(0,0,"Enter your Username: ");
  echo();
  getnstr(username, sizeof(username));//so they have ttheir actual username
  noecho();

  if(username[0] == '\0'){
    strcpy(username,"Guest");
  }

  clear();


  server_socket = client_tcp_handshake(IP);

  char join_msg[MAX_MSG_SIZE];
  snprintf(join_msg, sizeof(join_msg), "__JOIN__:%s\n", username);
  write(server_socket, join_msg, strlen(join_msg));

  int screen_h;
  int screen_w;

  getmaxyx(stdscr,screen_h,screen_w);

  int input_h = 3;
  int top_h = screen_h - input_h;//rest of the space goes to chat + userlist


  int users_w = 20;
  int chat_w = screen_w - users_w;//rest of this space goes to chat

  WINDOW * users_WIN = newwin(top_h,users_w,0,0);

  WINDOW * chat_WIN = newwin(top_h,chat_w,0,users_w);

  WINDOW * input_WIN = newwin(input_h,screen_w,top_h,0);

  idlok(chat_WIN,TRUE);
  scrollok(chat_WIN,TRUE);
  setscrreg(0,top_h);


  char user_LIST[50][33];
  int num_users = 0;

  char input[1025];
  int input_LEN = 0;

  input[0] = '\0';

  for(int i = 0;i < 50; i ++){
    user_LIST[i][0] = '\0';
  }

  strcpy(user_LIST[0],username);
  num_users += 1;


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

    select(server_socket + 1, &read_fds, NULL, NULL, NULL);


    if (FD_ISSET(server_socket, &read_fds)) {
      char msg[MAX_MSG_SIZE];

      int bytes = read(server_socket, msg, sizeof(msg));
      err(bytes, "read message from server");

      if (bytes == 0) {
        endwin();
        printf("Server disconnected.\n");
        close(server_socket);
        exit(0);
      }

      msg[bytes] = '\0';

      if (strncmp(msg,"__JOIN__:",9) == 0) {
        char temp[33];
        int k = 0;

        while (k < 32 && msg[9 + k] != '\0' && msg[9 + k] != '\n') {
          temp[k] = msg[9 + k];
          k++;
        }
        temp[k] = '\0';

        int exists = 0;
        for(int i = 0; i < num_users; i++){
          if(strcmp(user_LIST[i], temp) == 0){
            exists = 1;
          }
        }

        if(exists == 0){
          if(num_users < 50){
            strcpy(user_LIST[num_users], temp);
            num_users++;
          }
        }

        werase(users_WIN);
        box(users_WIN,0,0);
        mvwprintw(users_WIN,0,2,"User List ");

        for(int i =0;i < num_users && i < top_h -2; i ++){
          mvwprintw(users_WIN,1 + i,1,"%s",user_LIST[i]);
        }

        wrefresh(users_WIN);
      }

      else if (strncmp(msg,"__LEAVE__:",10) == 0) {
        char temp[33];
        int k = 0;

        while (k < 32 && msg[10 + k] != '\0' && msg[10 + k] != '\n') {
          temp[k] = msg[10 + k];
          k++;
        }
        temp[k] = '\0';

        for(int i = 0; i < num_users; i++){
          if(strcmp(user_LIST[i], temp) == 0){
            for(int j = i; j < num_users - 1; j++){
              strcpy(user_LIST[j], user_LIST[j + 1]);
            }
            num_users--;
            i--;
          }
        }

        werase(users_WIN);
        box(users_WIN,0,0);
        mvwprintw(users_WIN,0,2,"User List ");

        for(int i =0;i < num_users && i < top_h -2; i ++){
          mvwprintw(users_WIN,1 + i,1,"%s",user_LIST[i]);
        }

        wrefresh(users_WIN);
      }

      else {
        if(num_messages < MAX_MSGS){
          strcpy(messages[num_messages], msg);
          num_messages++;
        }

        werase(chat_WIN);
        box(chat_WIN,0,0);
        mvwprintw(chat_WIN,0,2,"Chat ");
        idlok(chat_WIN,TRUE);
        scrollok(chat_WIN,TRUE);
        // loop through all the messages, update ncurses

        for(int i = 0; i < num_messages && i < top_h - 2; i ++){
          mvwprintw(chat_WIN,i + 1, 1,"%s",messages[i]);
        }
        wrefresh(chat_WIN);
      }
    }


    if (FD_ISSET(STDIN_FILENO, &read_fds)) {

      int key = getch();

      if(key == KEY_BACKSPACE || key == 127){
        if(input_LEN > 0){
            input_LEN--;
            input[input_LEN] = '\0';
        }
      }

      if(key == '\n'){
        if(input_LEN > 0){

            char new_msg[MAX_MSG_SIZE];


            time_t raw_time;
            time(&raw_time);

            struct tm * local_time = localtime(&raw_time);

            char time[8];
            strftime(time, sizeof(time), "%I:%M%p", local_time);

            snprintf(new_msg, sizeof(new_msg), "%s: %s (%s)\n", username, input, time);

            int bytes = write(server_socket, new_msg, strlen(new_msg));
            err(bytes, "write message to server");

            input_LEN = 0;
            input[0] = '\0';
        }
      }

      if(key >= 32 && key <= 126){//if its a n actual letter add to end
        if(input_LEN < 1024){
            input[input_LEN] = (char)key;
            input_LEN ++;
            input[input_LEN] = '\0';
        }
      }

      if (key == KEY_LEFT && input_LEN > 0) {
        int cur_x; int cur_y;
        getyx(stdscr, cur_y, cur_x);
        move(cur_y, cur_x-1);
      }

      werase(input_WIN);
      box(input_WIN,0,0);
      mvwprintw(input_WIN,0,2,"Input ");
      mvwprintw(input_WIN,1,1,"| %s",input);
      wrefresh(input_WIN);

      werase(users_WIN);
      box(users_WIN,0,0);
      mvwprintw(users_WIN,0,2,"User List ");

      for(int i =0;i < num_users && i < top_h -2; i ++){
        mvwprintw(users_WIN,1 + i,1,"%s",user_LIST[i]);
      }

      wrefresh(users_WIN);

    }

  }

  close(server_socket);
  endwin();
  return 0;



}
