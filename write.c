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

		if((write(fd, file_name, strlen(file_name))) == strlen(file_name))
		{
			if((write(fd, &field_delimiter, 1)) == 1) // separator
			{
				if((write(fd, text, strlen(text))) == strlen(text))
				{
					if((write(fd, &field_delimiter, 1)) == 1) //separator
					{
						if((write(fd, date, strlen(date))) == strlen(date))
						{
							if((write(fd, &field_delimiter, 1)) ==1) //separator
							{
								if((write(fd, tags_raw, strlen(tags_raw))) == strlen(tags_raw))
								{
									if((write(fd, &entry_delimiter, 1)) == 1)
									{
										debug("Everything has been successfully written to the DATA_FILE");
									}
									else error(errmsg);
								}
								else error(errmsg);
							}
							else error(errmsg);
						}
						else error(errmsg);
					}
					else error(errmsg);
				}
				else error(errmsg);
			}
			else error(errmsg);
		}

		else error(errmsg);
	}

	else
	{

		error("Opening DATA_FILE");
	}
}
