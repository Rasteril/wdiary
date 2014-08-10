#include "proto.h"

void write_to_data_file(char *file_name, char *text, char *date, char *tags_raw, char saved_tags_array[][TAG_WORD_MAX_SIZE])
{
	int oflags = O_WRONLY;
	int fd;

	// Create or append to the DATA_FILE
	if(file_exists(DATA_FILE))
	{
		oflags |= O_APPEND;
	}

	else
	{
		oflags |= O_CREAT;
		debug("Creating data file at %s", DATA_FILE);
	}


	// write to data file
	if((fd = open(DATA_FILE, oflags, S_IWUSR | S_IRUSR)) != -1)
	{
		debug("Data file opened successfully");
		debug("text = %s", text);
		debug("filename = %s", file_name);
		debug("strlen(text) = %d", strlen(text));

		char *errmsg = "Writing to file";

		char field_delimiter = FIELD_DELIMITER; // constant -> local var, to enable & (get addr)
		char entry_delimiter = ENTRY_DELIMITER;


		// a lot of checks to guarantee data integrity
		
		// calculate length
		int entry_length = strlen(file_name)
		   					+ 1 /* = FIELD_DELIMITER */ 
							+ strlen(text) 
							+ 1 /* = FIELD_DELIMITER */ 
							+ strlen(date) 
							+ 1 /* = FIELD_DELIMITER */ 
							+ strlen(tags_raw) 
							+ 1; /* = ENTRY_DELIMITER*/

		char composed_string[entry_length];

		strncpy(composed_string, file_name, strlen(file_name));
		strncat(composed_string, &field_delimiter, 1);
		strncat(composed_string, text, strlen(text));
		strncat(composed_string, &field_delimiter, 1);
		strncat(composed_string, date, strlen(date));
		strncat(composed_string, &field_delimiter, 1);
		strncat(composed_string, tags_raw, strlen(tags_raw));
		strncat(composed_string, &entry_delimiter, 1);


		if(write(fd, composed_string, entry_length) == entry_length)
		{
			debug("Message successfully written to %s", DATA_FILE);
		}

		else 
		{
			debug("Error writing to %s", DATA_FILE);
			error("Writing to data file");
		}
	}

	else
	{

		error("Opening DATA_FILE");
	}
}
