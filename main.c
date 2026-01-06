#include <curses.h>

int main() {
  initscr();

  move(0, 0);
  addstr("hi");
  refresh();


  endwin();
  return 0;

}
