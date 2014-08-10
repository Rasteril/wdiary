#include "proto.h"

#include <regex.h>

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


