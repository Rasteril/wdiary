#include <unistd.h>
#include <time.h>
#include <fcntl.h> // for the open() oflags
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // for the file_exists function
#include <string.h>
#include <stdarg.h> // va_arg, indefinite parameter count


#define TRUE 1
#define FALSE 0
#define SIZE 1024

#define DEBUG_MODE TRUE

#define PROGRAM_NAME "wdiary"
#define FILE_NAME_MAX_SIZE 256


//Log functions
void debug(char *, ...);
void error(char *);

void usage();
char *get_date();
int file_exists(char *);
int process_args(int argc, char *argv[], char *file_name, char *text);

int main(int argc, char *argv[])
{
	char file_name[256];
	char text[SIZE];
	process_args(argc, argv, file_name, text);

	char *date = get_date();
	int oflags = O_WRONLY;
	int fd;
	char string[SIZE];
	
	if(file_exists(file_name))
	{
		oflags |= O_APPEND;
		printf("Appending to file '%s'\n", file_name);
		snprintf(string, SIZE, "\n\n%s\n\n------\n", text);

	}

	else
	{
		oflags |= O_CREAT;
		printf("\nCreating entry as file '%s'\n", file_name);
		snprintf(string, SIZE, "\nWDiary Note :: write date: [%s] :: write name: [%s]\n------\n\n%s\n\n------\n", date, file_name, text);

	}


	if((fd = open(file_name, oflags, S_IWUSR | S_IRUSR)) != -1)
	{
		debug("File opened sucessfully");
		debug("strlen(text) = %d", strlen(text));

		if((write(fd, string, strlen(string))) == strlen(string))
		{
			debug("Successfully written to file");
		}

		debug(text);
	}

	else
	{
		error("Opening file");
	}

	return EXIT_SUCCESS;

}

void usage() 
{
	printf("\nIncorrect usage!\n");
	printf("%s <text>\n", PROGRAM_NAME);
}


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
	if(DEBUG_MODE)
	{

		char msg[256];
		va_list list;
		sprintf(msg, "\n[DEBUG] %s \n", debug_msg);

		va_start(list, debug_msg);

		int argument = va_arg(list, int);
		printf(msg, argument);

		va_end(list);
	}
}

void error(char *error_msg)
{
	printf("\n[ERROR] %s\n", error_msg);
}

int process_args(int argc, char *argv[], char *file_name, char *text)
{
	int curr_arg;
	do
	{
		switch(curr_arg)
		{
			case 'n':
				strcpy(file_name, optarg);
				debug(optarg);
				break;

			case 'd':
				strcpy(file_name, get_date());
				break;

			case '?':
				debug("optopt ?");
				error("Invalid option");
				_exit(EXIT_FAILURE);
				break;

			default:

				if(argc < 2)
				{
					usage();
					_exit(EXIT_FAILURE);
				}


				else
				{
					strcpy(file_name, get_date());
					break;
				}
		}

	}
	while((curr_arg = getopt(argc, argv, "n:d")) != -1);

	strcpy(text, argv[optind]);
}
