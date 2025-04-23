#include "../../gui/cli/structs.h"
#include "backtetris.h"

void read_score(GameInfo_t *stat) {
  char str[7] = {0};
  if (stat->score == 0) {
    FILE *file = fopen("max_score.txt", "r");
    if (file) {
      fgets(str, 7, file);
      stat->high_score = atoi(str);
      fclose(file);
    }
  }
}

void write_score(GameInfo_t *stat) {
  char str[7] = {0};
  if (stat->score > stat->high_score) {
    FILE *file = fopen("max_score.txt", "w+");
    if (file) {
      sprintf(str, "%d", stat->score);
      fputs(str, file);
      fclose(file);
    }
  }
  stat->score = 0;
}

void level(GameInfo_t *stat) {
  if ((stat->score >= (600 * stat->level)) && stat->level < 10) {
    stat->level++;
  }
}

int is_empty_fieldline(int *line) {
  int res = 1;
  for (int x = 0; x < WIDTH && res; x++)
    if (line[x]) res = 0;
  return res;
}
int is_empty_line(int *line) {
  int res = 1;
  for (int x = 0; x < F_SIDE && res; x++)
    if (line[x]) res = 0;
  return res;
}

void alloc_field(GameInfo_t *stat) {
  stat->field = (int **)calloc((F_SIDE + HEIGHT), sizeof(int *));
  for (int y = 0; y < (HEIGHT + F_SIDE); y++) {
    stat->field[y] = (int *)calloc(WIDTH, sizeof(int));
  }
}

void alloc_next(GameInfo_t *stat) {
  stat->next = (int **)calloc(F_SIDE, sizeof(int *));
  for (int y = 0; y < F_SIDE; y++) {
    stat->next[y] = (int *)calloc(F_SIDE, sizeof(int));
  }
}

State *init_state_sctruct() {
  static State state = {0};
  return &state;
}