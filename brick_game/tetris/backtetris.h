#include <curses.h>

#include "../../gui/cli/structs.h"
// lib_interface
UserAction_t get_signal(State *state, chtype);
void sigact(State *, UserAction_t *, GameInfo_t *, figure_struct *);

// service / backend / lib_logic
void spawn(GameInfo_t *game, figure_struct *);
void moving(GameInfo_t *game, figure_struct *current);
int attaching(GameInfo_t *game, figure_struct *current);
void shifting(GameInfo_t *, figure_struct *, UserAction_t);
void move_down(figure_struct *figure, GameInfo_t *stat);
void rotation(figure_struct *figure, GameInfo_t *stat);
// generate / create / spawn
void gen_figure(GameInfo_t *stat, figure_struct *current);

// place_block
void input_figure(GameInfo_t *stat, figure_struct *current);

void remove_figure(GameInfo_t *stat, figure_struct *current);
void check_full_line(GameInfo_t *stat);
void del_full_line(GameInfo_t *stat, int number_line, int count_lines);

// helpers .c/.h
int is_empty_fieldline(int *line);
int is_empty_line(int *line);
void level(GameInfo_t *stat);  //
void read_score(GameInfo_t *stat);
void write_score(GameInfo_t *stat);
void count_score(int count_lines, int *score);

// memory_managent
State *init_state_sctruct();
void alloc_field(GameInfo_t *stat);
void alloc_next(GameInfo_t *stat);
void free_next(GameInfo_t *stat);
void free_field(GameInfo_t *stat);
void clear_field(GameInfo_t *game);
