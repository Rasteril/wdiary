#include "wdiary.h"

/*
 * UTILS.C
 */

//log functions
void debug(char *, ...);
void error(char *);

// util functions
char *get_date();
int file_exists(char *);
char *trim_spaces(char *string);

void substring(char *to, char *string, int begin, int count);
void display_message(char *message);

/*
 * MAIN.C
 */

void usage();
int process_args(int argc, char *argv[], char *file_name, char *text, char *tags_raw, char saved_tags_array[][TAG_WORD_MAX_SIZE]);

/*
 * BROWSE.C
 */

int start_gui_browse_mode(char *browse_mode);
int populate_data_array(char data_array[][FIELD_COUNT][FIELD_SIZE], int entry_offsets[ENTRY_MAX_COUNT]);
WINDOW *draw_menu(WINDOW *menu_win, int entry_count, char data_array[][FIELD_COUNT][FIELD_SIZE], int menu_pointer);
void menu_cursor(int *menu_pointer, char direction, int entry_count);
void show_entry_text(WINDOW *text_win, int entry_id, char data_array[][FIELD_COUNT][FIELD_SIZE]);
void delete_entry (int entry_id, char data_array[][FIELD_COUNT][FIELD_SIZE], int entry_offsets[ENTRY_MAX_COUNT]);
void reset_window(WINDOW *window);

/*
 * WRITE.C
 */

void write_to_data_file(char *file_name, char *text, char *date, char *tags_raw, char saved_tags_array[][TAG_WORD_MAX_SIZE]);

/*
 * WRITE_FORM.C
 */

int launch_write_form(struct form_results *fr);
void submit(struct form_results *pfr, FIELD **fields, FORM *main_form);

