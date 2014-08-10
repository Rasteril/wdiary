#include "proto.h"

int gui_write_mode = FALSE;

int main(int argc, char *argv[])
{
	// variables that are needed in the argument parsing process 
	
	struct form_results *pfr = &fr;

	char file_name[FILE_NAME_MAX_SIZE];
	char text[TEXT_MAX_SIZE];
	char tags_raw[TAGS_MAX_SIZE];
	char saved_tags_array[TAG_MAX_COUNT][TAG_WORD_MAX_SIZE];

	strcpy(file_name, "---"); // no filename, to be filtered. 
	process_args(argc, argv, file_name, text, tags_raw,  saved_tags_array);

	char *date = get_date();
	int oflags = O_WRONLY;
	int fd;

	if(gui_write_mode)
	{

		launch_write_form(pfr);
		write_to_data_file(pfr->file_name, pfr->text, date, pfr->tags_raw, saved_tags_array);
	}

	else
	{
		write_to_data_file(file_name, text, date, tags_raw, saved_tags_array);
	}


	return EXIT_SUCCESS;

}


int process_args(int argc, char *argv[], char *file_name, char *text, char *tags_raw, char saved_tags_array[][TAG_WORD_MAX_SIZE])
{
	int curr_arg = 0;
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

			case 't':
				strcpy(tags_raw, optarg);
				debug("Tags: %s", tags_raw);
				process_tags(saved_tags_array, tags_raw);
				break;
			case 'v':
				verbose_mode = TRUE;
				break;

			case 'g':
				printf("Started the arg\n");

				gui_write_mode = TRUE;
				break;

			case 'b':
				start_gui_browse_mode("date");
				break;

			case 'B':
				start_gui_browse_mode("tags");

			case '?':
				debug("optopt ?");
				error("Invalid option");
				_exit(EXIT_FAILURE);
				break;
		}

	}
	while((curr_arg = getopt(argc, argv, "bBgvn:t:d")) != -1);


	if(!gui_write_mode)
	{
		if(argc == optind) // there are as many arguments, as there are option arguments. This means the extra parameter for the actual message text is missing. 
		{
			usage();
			_exit(EXIT_FAILURE);
		}

		strcpy(text, argv[optind]);
	}
}



void usage() 
{
	printf("\nIncorrect usage!\n");
	printf("%s <text>\n", PROGRAM_NAME);
}


