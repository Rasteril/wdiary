#include "proto.h"

#include <termios.h>
#include <sys/ioctl.h>


int launch_write_form(struct form_results *pfr)
{
	// init ncurses
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	FIELD *fields[FORM_FIELD_COUNT];
	FORM *main_form;

	int ch, i;
	unsigned short field_width = COLS - 20;

	int startx = 15;
	int starty = 4;

	// filename
	fields[FORM_FILE_NAME_FIELD] = new_field(1, field_width, starty, startx, 0, 0);

	// text
	fields[FORM_TEXT_FIELD] = new_field(5, field_width, starty + 2, startx, 0, 0);

	// tags
	fields[FORM_TAGS_FIELD] = new_field(1, field_width, starty + 10, startx, 0, 0);
	fields[FORM_NULL_FIELD] = NULL; // last must be null, ALWAYS


	// set field bg
	for(i = 0; i < FORM_FIELD_COUNT - 1; i++)
	{
		set_field_back(fields[i], A_REVERSE);

		// make fields dynamically scalable
		field_opts_off(fields[i], O_STATIC);

		// prevent from skipping to another field when done full
		field_opts_off(fields[i], O_AUTOSKIP);

		set_max_field(fields[i], FIELD_SIZE);

	}

	main_form = new_form(fields);
	post_form(main_form);
	
	mvprintw(starty, startx - 10, "Filename: ");
	mvprintw(starty + 2, startx - 10, "Text: ");
	mvprintw(starty + 10, startx - 10, "Tags: ");
	refresh();

	while((ch = getch()) != KEY_F(1))
	{	switch(ch)
		{	
			case KEY_DOWN:
				form_driver(main_form, REQ_DOWN_CHAR);
				break;

			case KEY_UP:
				form_driver(main_form, REQ_UP_CHAR);
				break;

			case KEY_LEFT:
				form_driver(main_form, REQ_LEFT_CHAR);
				break;

			case KEY_RIGHT:
				form_driver(main_form, REQ_RIGHT_CHAR);
				break;

			case KEY_BACKSPACE:
				form_driver(main_form, REQ_DEL_PREV);
				break;
			
			case TAB_KEY:
				form_driver(main_form, REQ_NEXT_FIELD);
				break;
			
			case CTRL_E_KEY:
				form_driver(main_form, REQ_END_FIELD);
				form_term(main_form);
				break;
			
			case CTRL_A_KEY:
				form_driver(main_form, REQ_BEG_FIELD);
				break;

			case CTRL_K_KEY:
				form_driver(main_form, REQ_DEL_LINE);
				break;

			case CTRL_P_KEY:
				submit(pfr, fields, main_form);
				endwin();
				return EXIT_SUCCESS;

				break;

			case ENTER_KEY:
				display_message("Newlines not supported yet");
				break;


			default:
				// Normal characters get printed
				form_driver(main_form, ch);
				break;
		}
	}

	/* Un post form and free the memory */
	unpost_form(main_form);
	free_form(main_form);

	for(i = 0; i < FORM_FIELD_COUNT - 1; i++)
	{
		free_field(fields[i]);
	}

	endwin();
}

void submit(struct form_results *pfr, FIELD **fields, FORM *main_form)
{
	// request the correct values of all the form fields (even when still active in the field)
	form_driver(main_form, REQ_VALIDATION);
	strcpy(pfr->file_name, trim_spaces(field_buffer(fields[FORM_FILE_NAME_FIELD], 0)));
	strcpy(pfr->text, trim_spaces(field_buffer(fields[FORM_TEXT_FIELD], 0)));
	strcpy(pfr->tags_raw, trim_spaces(field_buffer(fields[FORM_TAGS_FIELD], 0)));

	display_message("Note successfully saved!");
	refresh();

}

