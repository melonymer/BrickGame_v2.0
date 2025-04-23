#include "structs.h"

int main() {
  srand(time(NULL));
  menu();
  refresh();
  endwin();
  return 0;
}

void menu() {
  okna windows = {0};
  GameInfo_t game = {0};
  alloc_field(&game);
  init_scr(&windows);
  cbreak();
  noecho();
  curs_set(0);
  keypad(windows.wnd, TRUE);
  nodelay(windows.wnd, TRUE);

  State *state = init_state_sctruct();
  *state = START;
  while (*state != EXIT) {
    if (*state == START) start_print(&windows);
    if (*state == MOVING) {
      delay_output(5);
      stat_print(&windows, game);
    }
    if (*state == GAME_OVER) GO_print(&windows);
    if (*state == PAUSE) pause_print(&windows);
    game = updateCurrentState();
  }
}

void pause_print(okna *windows) {
  clear_windows(windows);
  box_print(windows);
  mvwaddstr(windows->field, 6, 8, "PAUSE");
  mvwaddstr(windows->score, 1, 4, "SCORE:");
  mvwaddstr(windows->max_score, 1, 3, "MAX SCORE");
  mvwaddstr(windows->next, 1, 5, "NEXT");
}

void init_scr(okna *windows) {
  windows->wnd = initscr();
  wresize(windows->wnd, 24, 42);
  windows->field = subwin(windows->wnd, 22, 22, 1, 2);
  windows->score = subwin(windows->wnd, 4, 14, 1, 25);
  windows->max_score = subwin(windows->wnd, 4, 14, 5, 25);
  windows->next = subwin(windows->wnd, 8, 14, 9, 25);
  windows->speed = subwin(windows->wnd, 6, 14, 17, 25);
}

void start_print(okna *windows) {
  clear_windows(windows);
  box_print(windows);
  mvwaddstr(windows->field, 10, 1, "Press ENTER to start");
  mvwaddstr(windows->field, 11, 7, "ESC   to exit");
  mvwaddstr(windows->score, 1, 4, "SCORE:");
  mvwaddstr(windows->max_score, 1, 3, "MAX SCORE");
  mvwaddstr(windows->next, 1, 5, "NEXT");
}

void GO_print(okna *windows) {
  clear_windows(windows);
  mvwaddstr(windows->field, 9, 7, "GAME OVER");
  mvwaddstr(windows->field, 11, 4, "ENTER -> MENU");
  mvwaddstr(windows->field, 12, 6, "ESC -> EXIT");
  box_print(windows);
}

void clear_windows(okna *windows) {
  wclear(windows->field);
  wclear(windows->next);
  wclear(windows->score);
  wclear(windows->max_score);
  wclear(windows->speed);
}

void box_print(okna *windows) {
  box(windows->wnd, ACS_VLINE, ACS_HLINE);
  box(windows->field, ACS_VLINE, ACS_HLINE);
  box(windows->next, ACS_VLINE, ACS_HLINE);
  box(windows->score, ACS_VLINE, ACS_HLINE);
  box(windows->speed, ACS_VLINE, ACS_HLINE);
  box(windows->max_score, ACS_VLINE, ACS_HLINE);
  mvwaddch(windows->next, 2, 0, ACS_LTEE);
  mvwaddch(windows->next, 2, 13, ACS_RTEE);
  for (int i = 1; i < 13; i++) mvwaddch(windows->next, 2, i, ACS_HLINE);
}

void stat_print(okna *windows, GameInfo_t stat) {
  clear_windows(windows);
  box_print(windows);
  mvwaddstr(windows->score, 1, 4, "SCORE:");
  mvwprintw(windows->score, 2, 4, "%06d", stat.score);

  mvwaddstr(windows->max_score, 1, 3, "MAX SCORE");
  mvwprintw(windows->max_score, 2, 4, "%06d", stat.high_score);

  mvwaddstr(windows->next, 1, 5, "NEXT");
  int empty_lines = 0;
  int pole_y = 3, pole_x = 3;
  while (is_empty_line(stat.next[empty_lines])) empty_lines++;
  if (empty_lines > 1) pole_y--;

  for (int y = 0; y < F_SIDE; y++, pole_y++) {
    for (int x = 0; x < F_SIDE; x++) {
      if (stat.next[y][x]) {
        mvwaddch(windows->next, pole_y, (x * 2) + pole_x, BLOCK);
        mvwaddch(windows->next, pole_y, (x * 2) + pole_x + 1, BLOCK);
      }
    }
  }
  for (int y = 4; y < (HEIGHT + F_SIDE); y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (stat.field[y][x]) {
        mvwaddch(windows->field, y - 3, (x * 2) + 1, BLOCK);
        mvwaddch(windows->field, y - 3, (x * 2) + 2, BLOCK);
      } else {
        mvwaddch(windows->field, y - 3, (x * 2) + 1, ' ');
        mvwaddch(windows->field, y - 3, (x * 2) + 2, ' ');
      }
    }
  }

  mvwprintw(windows->speed, 2, 3, "SPEED %2d", stat.speed);
  mvwprintw(windows->speed, 3, 3, "LEVEL %2d", stat.level);
}
