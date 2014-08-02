#include "proto.h"


int verbose_mode = FALSE; // Global mode variable

char *get_date()
{
	time_t current_time;
	struct tm *tm_ptr;
	static char date[256];
	(void)time(&current_time);
	tm_ptr = localtime(&current_time);
	strftime(date, 256, "%d-%m-%Y", tm_ptr);
	return date;
	
}

int file_exists(char *filename)
{
	struct stat buffer;
	int ret = stat(filename, &buffer);
	if(ret == 0)
	{
		return TRUE;
	}
	
	else
	{
		return FALSE;
	}
}

void debug(char *debug_msg, ...)
{
	if(verbose_mode)
	{

		char msg[256];
		va_list list;
		sprintf(msg, "\n[DEBUG] %s \n", debug_msg);

		va_start(list, debug_msg);

		char *argument = va_arg(list, char *); // This works for ints too, because the argument must be big enough to hold the BIGGEST argument. 
		printf(msg, argument);

		va_end(list);
	}

	else 
	{
		; // do nothing, verbose_mode == FALSE;
	}
}

void error(char *error_msg)
{
	printf("\n[ERROR] %s\n", error_msg);
}


char *trim_spaces(char *string)
{
	char *end;
	//trim from beginning to first
	while(isspace(*string)) string++;

	if(*string == 0) return string; //string all spaces

	end = string + strlen(string) - 1;

	//trim from end to last
	while(end > string && isspace(*end)) end--;

	*(end + 1) = 0; // null terminate the string;

	return string;
}

void display_message(char *message)
{
	WINDOW *message_win;
	int i, height, width, startx, starty;

	// Set properties
	width = 5 + strlen(message);
	height = 5;
	startx = (COLS - width) / 2;
	starty =  5;//(LINES - height) / 2;

	message_win = newwin(height, width, starty, startx);
	box(message_win, 0, 0);

	// padding
	wmove(message_win, 2, 2);
	wprintw(message_win, message);
	wrefresh(message_win);

	sleep(1);

	delwin(message_win);
	touchwin(stdscr);
	//refresh();


}

void substring(char *to, char *string, int begin, int count)
{
	strncpy(to, string + begin, count);
}
