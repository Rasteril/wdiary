/*
 * INCLUDES
 */

#include <ncurses.h>
#include <form.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/*
 * DEFINES
 */

#define TRUE 1
#define FALSE 0

#define PROGRAM_NAME "wdiary"
#define DATA_FILE "entries.data"

// main.c
#define ENTRY_MAX_COUNT 200
#define FIELD_COUNT 4
#define FIELD_SIZE 1024
#define TEXT_MAX_SIZE FIELD_SIZE //1024
#define FILE_NAME_MAX_SIZE FIELD_SIZE
#define TAGS_MAX_SIZE FIELD_SIZE

#define TAG_WORD_MAX_SIZE 20
#define TAG_MAX_COUNT 20

#define FIELD_DELIMITER '\16'
#define ENTRY_DELIMITER '\17'

#define STARTX 15
#define STARTY 4

#define FORM_FIELD_COUNT 4

// custom key bindings (where ncurses was lacking)
#define TAB_KEY 9
#define CTRL_A_KEY 1
#define CTRL_E_KEY 5
#define CTRL_K_KEY 11
#define CTRL_P_KEY 16
#define ENTER_KEY 10
#define SPACE 0x20

/*
 * GLOBALS
 */

extern int verbose_mode;
//extern int gui_write_mode;


/*
 * ENUMS
 */

enum form_field_names
{
   FORM_FILE_NAME_FIELD,
   FORM_TEXT_FIELD,
   FORM_TAGS_FIELD,
   FORM_NULL_FIELD
};


enum  // field index constants
{
	D_FILE_NAME,
	D_TEXT,
	D_DATE,
	D_TAGS
};

enum color_pairs
{
	COL_PAIR_HEADING = 1, 
	COL_PAIR_MENU_ITEM, 
	COL_PAIR_MENU_SHORT_TEXT
};

/*
 * STRUCTS
 */


struct field_prop
{
	int height;
	char buffer[5000];
};

struct form_results
{
	char file_name[FILE_NAME_MAX_SIZE];
	char text[TEXT_MAX_SIZE];
	char tags_raw[TAGS_MAX_SIZE];
} fr;
