#include "../../gui/cli/tetris.h"

#include "../../gui/cli/structs.h"
#include "backtetris.h"

GameInfo_t updateCurrentState() {
  static GameInfo_t game = {0};
  if (!game.field) alloc_field(&game);
  State *state = init_state_sctruct();
  UserAction_t action = {0};
  static figure_struct current = {0};

  remove_figure(&game, &current);
  action = get_signal(state, getch());
  sigact(state, &action, &game, &current);
  input_figure(&game, &current);

  if (*state == EXIT) {
    free_next(&game);
    free_field(&game);
  }
  return game;
}

UserAction_t get_signal(State *state, chtype ch) {
  UserAction_t res = 0;
  if (*state == START && ch == ENTER) {
    res = Start;
  } else if (ch == 'p' || ch == 'P')
    res = Pause;
  else if (ch == ESC)
    res = Terminate;
  else if (ch == KEY_LEFT)
    res = Left;
  else if (ch == KEY_RIGHT)
    res = Right;
  else if (ch == KEY_DOWN)
    res = Down;
  else if (ch == KEY_UP)
    res = Up;
  else if (ch == ENTER)
    res = Action;
  else if (ch == (chtype)ERR)
    res = Nosig;
  else
    res = Nosig;

  return res;
}

void sigact(State *state, UserAction_t *action, GameInfo_t *game,
            figure_struct *current) {
  if (*state == START) {
    if (*action == Start) {
      spawn(game, current);
      moving(game, current);
      *state = MOVING;
    } else if (*action == Terminate)
      *state = EXIT;
  } else if (*state == MOVING) {
    if (*action == Down || *action == Left || *action == Right || *action == Up)
      shifting(game, current, *action);
    else if (*action == Action)
      rotation(current, game);
    else if (*action == Pause)
      *state = PAUSE;
    else if (*action == Terminate)
      *state = GAME_OVER;
    moving(game, current);
  } else if (*state == GAME_OVER) {
    write_score(game);
    if (*action == Nosig)
      ;
    else if (*action == Action) {
      *state = START;
      clear_field(game);
    } else if (*action == Terminate)
      *state = EXIT;
  } else if (*state == PAUSE) {
    if (*action == Pause) {
      *state = MOVING;
      *action = Nosig;
      moving(game, current);
    } else if (*action == Terminate)
      *state = GAME_OVER;
  }
}

void spawn(GameInfo_t *game, figure_struct *current) {
  game->level = 1;
  game->speed = 1;
  read_score(game);
  gen_figure(game, current);
}

void moving(GameInfo_t *game, figure_struct *current) {
  State *state = init_state_sctruct();
  level(game);
  for (int y = 0; y < F_SIDE; y++) {
    for (int x = 0; x < F_SIDE; x++) {
      if (current->figure[y][x])
        game->field[y + current->y][x + current->x] = 0;
    }
  }
  switch (attaching(game, current)) {
    case 1:
      input_figure(game, current);
      gen_figure(game, current);
      check_full_line(game);
      break;
    case 2:
      *state = GAME_OVER;
      break;
  }
  move_down(current, game);
}

void shifting(GameInfo_t *game, figure_struct *current, UserAction_t action) {
  int empty_l_col = 0;
  int empty_r_col = 0;
  int buf = 0, nospace = 0;
  for (int x = 0; x < F_SIDE; x++) {
    buf = 0;
    for (int y = 0; y < F_SIDE; y++) {
      if (!current->figure[y][x]) buf++;
    }
    if (buf == F_SIDE && x < 2) empty_l_col++;
    if (buf == F_SIDE && x > 1) empty_r_col++;
  }
  for (int y = 0; y < F_SIDE; y++) {
    for (int x = 0; x < F_SIDE; x++) {
      if (action == Left && (current->figure[y][x] &&
                             game->field[y + current->y][x + current->x - 1]))
        nospace = 1;
      if (action == Right && (current->figure[y][x] &&
                              game->field[y + current->y][x + current->x + 1]))
        nospace = 1;
    }
  }
  if (action == Left && ((current->x + empty_l_col) > 0) && !nospace)
    current->x--;
  if (action == Right && (current->x - empty_r_col < (WIDTH - F_SIDE)) &&
      !nospace)
    current->x++;
  if (action == Down)
    while (!attaching(game, current)) current->y++;
}

int attaching(GameInfo_t *game, figure_struct *current) {
  int res = 0;
  int kef = is_empty_line(current->figure[3]);
  if (current->y >= (HEIGHT + kef)) {
    res = 1;
  }
  if (!res) {
    for (int y = 0; (y < F_SIDE && !kef) || (y < (F_SIDE - 1) && kef); y++) {
      for (int x = 0; x < F_SIDE; x++) {
        if (game->field[y + current->y + 1][x + current->x] &&
            current->figure[y][x])
          res = 1;
      }
    }
  }
  if (res && !is_empty_fieldline(game->field[3])) res = 2;
  return res;
}

void gen_figure(GameInfo_t *stat, figure_struct *current) {
  static int figure = -1, state = 0;
  if (figure < 0) {
    figure = rand() % 7;
    state = rand() % 4;
    for (int y = 0; y < F_SIDE; y++) {
      for (int x = 0; x < F_SIDE; x++) {
        current->figure[y][x] = figure_sample[figure][state][y][x];
        // current->figure[y][x] = figure_sample[0][1][y][x];
      }
    }
  }
  if (figure >= 0) {
    for (int y = 0; y < F_SIDE; y++) {
      for (int x = 0; x < F_SIDE; x++) {
        current->figure[y][x] = figure_sample[figure][state][y][x];
        // current->figure[y][x] = figure_sample[0][1][y][x];
      }
    }
  }
  current->x = 3;
  if (is_empty_line(current->figure[3]))
    current->y = 1;
  else
    current->y = 0;
  current->fig = figure;
  current->state = state;
  figure = rand() % 7;
  state = time(NULL) % 4;
  if (stat->next == NULL) {
    alloc_next(stat);
  }
  for (int y = 0; y < F_SIDE; y++) {
    for (int x = 0; x < F_SIDE; x++) {
      stat->next[y][x] = figure_sample[figure][state][y][x];
      // stat->next[y][x] = figure_sample[0][1][y][x];
    }
  }
}

void move_down(figure_struct *figure, GameInfo_t *stat) {
  static int iteration = L1;
  if (iteration-- <= 0) {
    switch (stat->level) {
      case 1:
        iteration = L1;
        break;
      case 2:
        iteration = L2;
        break;
      case 3:
        iteration = L3;
        break;
      case 4:
        iteration = L4;
        break;
      case 5:
        iteration = L5;
        break;
      case 6:
        iteration = L6;
        break;
      case 7:
        iteration = L7;
        break;
      case 8:
        iteration = L8;
        break;
      case 9:
        iteration = L9;
        break;
      case 10:
        iteration = L10;
        break;
    }
    figure->y++;
  }
}

void input_figure(GameInfo_t *stat, figure_struct *current) {
  for (int y = 0; y < F_SIDE; y++) {
    for (int x = 0; x < F_SIDE; x++) {
      if (current->figure[y][x])
        stat->field[y + current->y][x + current->x] = 1;
    }
  }
}

void remove_figure(GameInfo_t *stat, figure_struct *current) {
  for (int y = 0; y < F_SIDE; y++) {
    for (int x = 0; x < F_SIDE; x++) {
      if (current->figure[y][x])
        stat->field[y + current->y][x + current->x] = 0;
    }
  }
}

void rotation(figure_struct *figure, GameInfo_t *stat) {
  figure_struct temp = {0};
  int collision = 0, buf = 0;
  int empty_l_col = 0, empty_r_col = 0;

  temp.x = figure->x;
  temp.y = figure->y;
  temp.fig = figure->fig;
  temp.state = (figure->state + 1) % 4;
  for (int y = 0; y < F_SIDE; y++) {
    for (int x = 0; x < F_SIDE; x++) {
      temp.figure[y][x] = figure_sample[temp.fig][temp.state][y][x];
    }
  }

  for (int x = 0; x < F_SIDE; x++) {
    buf = 0;
    for (int y = 0; y < F_SIDE; y++) {
      if (!figure->figure[y][x]) buf++;
    }
    if (buf == F_SIDE && x < 2) empty_l_col++;
    if (buf == F_SIDE && x > 1) empty_r_col++;
  }

  for (int y = 0; y < F_SIDE; y++) {
    for (int x = 0; x < F_SIDE; x++) {
      if (temp.figure[y][x] && stat->field[y + temp.y][x + temp.x])
        collision = 1;
    }
  }

  if (!collision) figure->state = temp.state;

  for (int y = 0; y < F_SIDE && !collision; y++) {
    for (int x = 0; x < F_SIDE; x++) {
      figure->figure[y][x] = figure_sample[figure->fig][figure->state][y][x];
      if (figure->figure[y][x] && ((figure->x + x) >= WIDTH)) figure->x--;
      if (figure->figure[y][x] && (figure->x + empty_l_col <= 0))
        figure->x += empty_l_col;
    }
  }
}

void free_next(GameInfo_t *stat) {
  for (int y = 0; y < F_SIDE; y++) {
    free(stat->next[y]);
  }
  free(stat->next);
}

void free_field(GameInfo_t *stat) {
  for (int y = 0; y < (HEIGHT + F_SIDE); y++) {
    free(stat->field[y]);
  }
  free(stat->field);
}

void clear_field(GameInfo_t *game) {
  for (int y = 0; y < (HEIGHT + F_SIDE); y++) {
    for (int x = 0; x < WIDTH; x++) {
      game->field[y][x] = 0;
    }
  }
}

void check_full_line(GameInfo_t *stat) {
  int count_lines = 0, number_line = 0;
  for (int y = (HEIGHT + F_SIDE - 1); y >= F_SIDE; y--) {
    int temp = 0;
    for (int x = 0; x < WIDTH; x++) {
      if (stat->field[y][x]) temp++;
    }
    if (temp == WIDTH) {
      if (!count_lines) number_line = y;
      count_lines++;
    }
  }
  if (count_lines) del_full_line(stat, number_line, count_lines);
}

void del_full_line(GameInfo_t *stat, int number_line, int count_lines) {
  int temp = count_lines;
  for (int y = number_line; count_lines; count_lines--, y--) {
    for (int x = 0; x < WIDTH; x++) {
      stat->field[y][x] = 0;
    }
  }
  count_lines = temp;
  for (int y = number_line; y >= F_SIDE; y--) {
    for (int x = 0; x < WIDTH; x++) {
      stat->field[y][x] = stat->field[y - temp][x];
    }
  }
  count_score(count_lines, &stat->score);
}

void count_score(int count_lines, int *score) {
  switch (count_lines) {
    case 1:
      *score += 100;
      break;
    case 2:
      *score += 300;
      break;
    case 3:
      *score += 700;
      break;
    case 4:
      *score += 1500;
      break;
  }
}