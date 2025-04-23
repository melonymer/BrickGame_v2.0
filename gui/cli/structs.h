#ifndef DEFINES_H
#define DEFINES_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 10
#define HEIGHT 20

#define ENTER 10
#define ESC 27

#define F_SIDE 4
#define BLOCK 32 | A_REVERSE

typedef struct {
  int x, y, fig, state;
  int figure[F_SIDE][F_SIDE];
} figure_struct;

typedef enum {
  L1 = 50,
  L2 = 46,
  L3 = 42,
  L4 = 38,
  L5 = 34,
  L6 = 30,
  L7 = 25,
  L8 = 20,
  L9 = 15,
  L10 = 10
} levels;
typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

typedef struct {
  WINDOW *wnd;
  WINDOW *field;
  WINDOW *score;
  WINDOW *max_score;
  WINDOW *next;
  WINDOW *speed;
} okna;

typedef enum {
  Start = 1,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action,
  Nosig
} UserAction_t;

typedef enum { START = 0, PAUSE, MOVING, GAME_OVER, EXIT } State;

int is_empty_line(int *line);
int noempty_col(int **figure, int col);
void menu();
void box_print(okna *windows);
void init_scr(okna *windows);
GameInfo_t updateCurrentState();
State *init_state_sctruct();
void start_print(okna *windows);
void GO_print(okna *windows);
void pause_print(okna *windows);
void stat_print(okna *windows, GameInfo_t stat);
void alloc_field(GameInfo_t *stat);
void clear_windows(okna *windows);

#endif  // DEFINES_H
