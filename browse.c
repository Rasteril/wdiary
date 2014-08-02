#include "proto.h"

#include <termios.h>
#include <sys/ioctl.h>


int start_gui_browse_mode()
{
	char data_array[ENTRY_MAX_COUNT][FIELD_COUNT][FIELD_SIZE];
	int entry_count;
	WINDOW *menu_win, *text_win;
	int menu_pointer = 0;
	int c;
	// init ncurses
	
	initscr();
	noecho();
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);
	start_color();

	init_color(COLOR_MAGENTA, 200, 200, 200); // make it grey
	init_pair(COL_PAIR_HEADING, COLOR_RED, COLOR_BLACK);
	init_pair(COL_PAIR_MENU_ITEM, COLOR_BLACK, COLOR_GREEN);
	init_pair(COL_PAIR_MENU_SHORT_TEXT, COLOR_MAGENTA, COLOR_BLACK);

	
	move(1, 2);
	attron(COLOR_PAIR(COL_PAIR_HEADING));
	addstr("WDiary BROWSE");	
	attroff(COLOR_PAIR(COL_PAIR_HEADING));
	printw(" mode (by date)");
	refresh();

	entry_count = populate_data_array(data_array);

	int startx, starty, width, height;
	width = COLS / 2 - 4;
	height = LINES - 10;
	startx = 2;
	starty = 2;

	menu_win = newwin(height, width, starty, startx);
	box(menu_win, 0, 0);

	text_win = newwin(LINES - 10, width, starty, startx + width + 3);

	draw_menu(menu_win, entry_count, data_array, menu_pointer);

	while((c = getch()) != 'q')
	{
		switch(c)
		{
			case 'k':
				menu_cursor(&menu_pointer, '+', entry_count);
				break;

			case 'j':
				menu_cursor(&menu_pointer, '-', entry_count);
				break;

			case 10:
				show_entry_text(text_win, menu_pointer, data_array);
				break;
		}
		draw_menu(menu_win, entry_count, data_array, menu_pointer);
	}
	
	endwin();

	return EXIT_SUCCESS;
}

int populate_data_array(char data_array[][FIELD_COUNT][FIELD_SIZE])
{
	int entry_count = 0;
	int field_count = 0;
	int fd = 0;
	char c;
	int i = 0;

	if((fd = open(DATA_FILE, O_RDONLY, S_IRUSR)) != -1)
	{
		// loop till the end of DATA_FILE
		while(read(fd, &c, 1) == 1)
		{
			if(c == FIELD_DELIMITER)
			{
				field_count++;
				data_array[entry_count][field_count][i] = 0; // terminate the string

				//reset
				i = 0;
			}

			else if (c == ENTRY_DELIMITER)
			{
				entry_count++;
				data_array[entry_count][field_count][i] = 0;
				
				//reset
				field_count = 0;
				i = 0;
			}

			else
			{
				data_array[entry_count][field_count][i] = c;
				i++;
			}
		}
	}

	else 
	{
		printf("Error opening file\n");
	}

	return entry_count;
}

WINDOW *draw_menu(WINDOW *menu_win, int entry_count, char data_array[][FIELD_COUNT][FIELD_SIZE], int menu_pointer)
{
	int i, j;
	int entry_ypos;
	entry_ypos = 2;
	wmove(menu_win, entry_ypos, 2); // add some padding

	for(i = 0; i < entry_count; i++)
	{
		int pre_size = (9 + strlen(data_array[i][D_DATE]) + 2 + strlen(data_array[i][D_FILE_NAME]));
		int short_text_size = pre_size < (COLS / 2) ? (COLS / 2) - pre_size : 0;
		char * short_text = malloc(short_text_size * sizeof(char));
		if(strlen(data_array[i][D_TEXT]) > short_text_size)
		{
			substring(short_text, data_array[i][D_TEXT], 0, short_text_size);
		}

		else
		{
			strcpy(short_text, data_array[i][D_TEXT]);
		}
		if(menu_pointer == i)
		{
			wattron(menu_win, COLOR_PAIR(COL_PAIR_MENU_ITEM));
		}

		wprintw(menu_win, "%s(%s) ", data_array[i][D_DATE],data_array[i][D_FILE_NAME]);
		wattron(menu_win, COLOR_PAIR(COL_PAIR_MENU_SHORT_TEXT));
		waddstr(menu_win,  short_text);
		wattroff(menu_win, COLOR_PAIR(COL_PAIR_MENU_SHORT_TEXT));

		wattroff(menu_win, COLOR_PAIR(COL_PAIR_MENU_ITEM));
		wmove(menu_win, ++entry_ypos, 2);
		free(short_text);
	}

	wrefresh(menu_win);
	return menu_win;

}

void menu_cursor(int *menu_pointer, char direction, int entry_count)
{
	switch(direction)
	{
		case '+':
			if(*menu_pointer > 0)
			{
				*menu_pointer = *menu_pointer - 1;
			}

			else 
			{
				*menu_pointer = entry_count - 1;
			}

			break;

		case '-':
			if(*menu_pointer < entry_count - 1)
			{
				*menu_pointer = *menu_pointer + 1;
			}

			else
			{
				*menu_pointer = 0;
			}
			break;
	}
}
			


void show_entry_text(WINDOW *text_win, int entry_id, char data_array[][FIELD_COUNT][FIELD_SIZE])
{
	wclear(text_win);

	mvwprintw(text_win, 2, 4, data_array[entry_id][D_TEXT]);
	wrefresh(text_win);
}
