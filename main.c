#include "proto.h"
#include <regex.h>

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

	printf("Out of process_args\n");

	if(gui_write_mode)
	{
		printf("in the check!\n");

		//printf("%i is the sizeof\n", sizeof(pfr->file_name[0]));
		pfr->file_name[0] = 'F';

		launch_write_form(pfr);
		printf("Reached HERE");
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
				start_gui_browse_mode();
				break;

			case '?':
				debug("optopt ?");
				error("Invalid option");
				_exit(EXIT_FAILURE);
				break;
		}

	}
	while((curr_arg = getopt(argc, argv, "bgvn:t:d")) != -1);


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


// PROCESS TAGS transform the tags_raw string variable into an array of single strings, ommiting any commas inbetween. 

int process_tags(char res_array[][20], char *tags_raw)
{
	int i, j, valid, tag_num, previ;
	tag_num = 0;
	previ = 0;
	valid = check_tags(tags_raw);

	if(!valid) // tags_raw consists of alphanumeric characters and commas only
	{
		error("Invalid tags text");
		_exit(EXIT_FAILURE);
	}

	else // passes regex control
	{
		debug("passed");
		for(i = 0; i <= strlen(tags_raw); i++) // loop through all of the tag characters
		{
			debug("i = %d", i);
			debug("%c", tags_raw[i]);
			debug("tags_raw[strlen(tags_raw)] = %d", tags_raw[strlen(tags_raw)]);
			if(tags_raw[i] != ',' && i != strlen(tags_raw)) // find delimiter ( character that separates the individual tag entries )
			{
				continue;
			}

			else // (delimiter) comma found
			{

				debug(" i = %d, previ = %d", i, previ);
				
				for(j = 0; j < (i - previ); j++) // put all the characters from the last delimiter (or beginning) to the current value of i (the position of the current delimiter)
				// j goes from 0 to i, the 2nd dimension is always (i - previ) characters big
				{
					// TODO  make the res_array big enough to hold all the tag things. Maybe, expand it gradually

					res_array[tag_num][j] = tags_raw[j + previ]; // insert into `tag_num`th res_array 1st dimensionelement and jth 2nd dimension element of res_array that (i - previ) characters starting from previ and going towars (previ + j);
				}
				tag_num++; // start pushing the new tag characters into another array in the second dimension
				previ = i + 1;
			}
		}
	}
}

int check_tags(char *tags_raw)
{
	int ret;
	regex_t regex;

	// compile the regular expression
	ret = regcomp(&regex, "^[a-zA-Z,]*$", 0);
	if(ret == TRUE) return FALSE;

	// execute
	ret = regexec(&regex, tags_raw, 0, NULL, 0);

	if(ret == FALSE)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


