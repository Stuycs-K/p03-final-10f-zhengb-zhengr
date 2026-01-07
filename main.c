#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
    initscr();
    cbreak();
    noecho();

    clear();

    printw("hello");
    refresh();

    WINDOW *test = newwin(3,50,6,2);///heihgt width x y
    box(test,0,0);

    mvwprintw(test, 1, 1, "> ");//moves cursor to inside window
    wrefresh(test);//refreshes only the window

    char buffer[128];

    echo();//echo on so I can see the typing
    wgetnstr(test, buffer, 128 - 1);//gets input from the window
    noecho();

   
    mvprintw(10, 2, "You entered: %s", buffer);
    mvprintw(12, 2, "Press any key to exit...");
    refresh();

    getch();  //waits for any key pess

  
    endwin();  

    return 0;
}