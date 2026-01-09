#include "networking.h"
#include <ncurses.h>

int main(){
  initscr();
  cbreak();
  noecho();

  int cols;
  int rows;

  getmaxyx(stdscr,cols,rows);

  WINDOW * input = newwin(3,50,50,0);
  WINDOW * chat = newwin(50,50,0,49);
  WINDOW * user_list = newwin(20,75,100,0);

  wclear(chat);
  box(chat,0,0);
  wmove(chat,true,true);
  wprintw(chat,"This is the chat");
  wrefresh(chat);

  wclear(input);
  wprintw(input,"| ");
  wrefresh(input);


scrollok(chat,TRUE);
scrollok(user_list,TRUE);

endwin();


}
