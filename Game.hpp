#include <iostream>
#include <ncurses.h>
#include <unistd.h>

#include "Player.hpp"

typedef enum {
  EMPTY,
  PLAYER_1,
  PLAYER_2
} Condition;

int win_positions[][3] = {
  {0, 1, 2},
  {3, 4, 5},
  {6, 7, 8},
  {0, 4, 8},
  {2, 4, 6},
  {0, 3, 6},
  {2, 5, 8},
};

char get_cond_char(Condition cond) {
  switch (cond) {
    case EMPTY:
      return ' ';
    case PLAYER_1:
      return 'O';
    case PLAYER_2:
      return 'X';
    default:
      return '?';
  }
}

class Game {
  private:
    Player pl1, pl2;
    Condition condition[9] = {};

    int make_move(Player *pl, int cell) {
      if (condition[cell]) {
        return 1;
      }
      condition[cell] = (pl == &pl1) ? PLAYER_1 : PLAYER_2;
      return 0;
    }

    void render() {
      int i, j;
      int cell_counter = 0;
      Condition cond;

      for (i = 0; i < 3; i++) {

        printw("+---++---++---+\n");

        for (j = 0; j < 3; j++) {
          cond = condition[cell_counter++];
          printw("| %c |", get_cond_char(cond));
        }
        printw("\n");

        printw("+---++---++---+\n");
      }
    }

    Player *search_winner() {
      int i = 0;

      for (i = 0; i < (sizeof win_positions / sizeof win_positions[0]); i++) {

        if ( condition[win_positions[i][0]] == PLAYER_1
            && condition[win_positions[i][1]] == PLAYER_1
            && condition[win_positions[i][2]] == PLAYER_1 ) {
          return &pl1;
        }

        if ( condition[win_positions[i][0]] == PLAYER_2
            && condition[win_positions[i][1]] == PLAYER_2
            && condition[win_positions[i][2]] == PLAYER_2 ) {
          return &pl2;
        }
      }

      return NULL;
    }

  public:
    Game() {
      pl1 = pl2 = Player();

      // TODO: Добавить возможность ввести имя
      pl1.name = "Fizz";
      pl2.name = "Buzz";
    }

    int run() {
      int input;
      Player *winner;
      Player *pl = &pl1;

      initscr();      // Переход в curses-режим

      while (true) {
        clear();

        render();

        printw("\n%s (%c)> ",
          pl->name.data(),
          get_cond_char((pl == &pl1 ? PLAYER_1 : PLAYER_2))
        );
        refresh();

        scanw("%d", &input);
        
        if ((make_move(pl, input-1))) {
          addstr("Error!\n");
          getch();
        } else {
          winner = search_winner();
          if (winner) {
            printw("Winner: %s!\n", pl->name.data());
            refresh();
            getch();
            break;
          } else {
            pl = (pl == &pl1) ? &pl2 : &pl1;
          }
        }
      }

      return endwin();
    }
};
