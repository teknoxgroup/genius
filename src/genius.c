/* GnomENIUS Calculator
 * Copyright (C) 1997, 1998, 1999 the Free Software Foundation.
 *
 * Author: George Lebl
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the  Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 * USA.
 */

/*
 * this is a command line genius interface program!
 */

#include "config.h"

#ifdef GNOME_SUPPORT
#include <gnome.h>
#else
#include <libintl.h>
#define _(x) gettext(x)
#endif

#include <glib.h>

#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include "calc.h"
#include "util.h"
#include "dict.h"
#include "inter.h"

#include <readline/readline.h>
#include <readline/history.h>

/*Globals:*/

/*calculator state*/
calcstate_t curstate={
	256,
	0,
	FALSE,
	FALSE
	};
	
extern calc_error_t error_num;
extern int got_eof;
extern int parenth_depth;

extern int interrupted;

static int use_readline = TRUE;

static void
puterror(char *s)
{
	char *file;
	int line;
	get_file_info(&file,&line);
	if(file)
		fprintf(stderr,"%s:%d: %s\n",file,line,s);
	else if(line>0)
		fprintf(stderr,"line %d: %s\n",line,s);
	else
		fprintf(stderr,"%s\n",s);
}

static void
set_state(calcstate_t state)
{
	curstate = state;
}

static void
interrupt(int sig)
{
	interrupted = TRUE;
	if(use_readline)
		rl_stuff_char('\n');
	signal(SIGINT,interrupt);
}

static int
nop(void)
{
	usleep(10000);
	return 0;
}

int
main(int argc, char *argv[])
{
	int i;
	int inter;
	int lastarg = FALSE;
	GList *files = NULL;
	char *file;
	FILE *fp;
	int do_compile = FALSE;
	int be_quiet = FALSE;

#ifdef GNOME_SUPPORT
	bindtextdomain(PACKAGE,GNOMELOCALEDIR);
#else
	bindtextdomain(PACKAGE,LOCALEDIR);
#endif
	textdomain(PACKAGE);

	signal(SIGINT,interrupt);

	statechange_hook = set_state;

	for(i=1;i<argc;i++) {
		int val;
		if(lastarg || argv[i][0]!='-')
			files = g_list_append(files,argv[i]);
		else if(strcmp(argv[i],"--")==0)
			lastarg = TRUE;
		else if(sscanf(argv[i],"--precision=%d",&val)==1)
			curstate.float_prec = val;
		else if(sscanf(argv[i],"--maxdigits=%d",&val)==1)
			curstate.max_digits = val;
		else if(strcmp(argv[i],"--floatresult")==0)
			curstate.results_as_floats = TRUE;
		else if(strcmp(argv[i],"--nofloatresult")==0)
			curstate.results_as_floats = FALSE;
		else if(strcmp(argv[i],"--scinot")==0)
			curstate.scientific_notation = TRUE;
		else if(strcmp(argv[i],"--noscinot")==0)
			curstate.scientific_notation = FALSE;
		else if(strcmp(argv[i],"--readline")==0)
			use_readline = TRUE;
		else if(strcmp(argv[i],"--noreadline")==0)
			use_readline = FALSE;
		else if(strcmp(argv[i],"--compile")==0)
			do_compile = TRUE;
		else if(strcmp(argv[i],"--nocompile")==0)
			do_compile = FALSE;
		else if(strcmp(argv[i],"--quiet")==0)
			be_quiet = TRUE;
		else if(strcmp(argv[i],"--noquiet")==0)
			be_quiet = FALSE;
		else {
			if(strcmp(argv[i],"--help")!=0) {
				fprintf(stderr,"Unknown argument '%s'!\n\n",
					argv[i]);
			}
			printf("Genius %s usage:\n\n"
			       "genius [options] [files]\n\n"
			       "\t--precision=num   \tFloating point precision [256]\n"
			       "\t--maxdigits=num   \tMaximum digits to display (0=no limit) [0]\n"
			       "\t--[no]floatresult \tAll results as floats [OFF]\n"
			       "\t--[no]scinot      \tResults in scientific notation [OFF]\n"
			       "\t--[no]readline    \tUse readline even if it is available [ON]\n"
			       "\t--[no]compile     \tCompile everything and dump it to stdout [OFF]\n"
			       "\t--[no]quiet       \tBe quiet during non-interactive mode,\n"
			       "\t                  \t(always on when compiling) [OFF]\n\n",
			      VERSION);
			exit(1);
		}
	}
	if(do_compile)
		be_quiet = TRUE;
	inter = isatty(0) && !files && !do_compile;
	/*interactive mode, print welcome message*/
	if(inter) {
		printf(_("Genius %s\n"
		     "Copyright (c) 1997,1998,1999 Free Software Foundation, Inc.\n"
		     "This is free software with ABSOLUTELY NO WARRANTY.\n"
		     "For details type `warranty'.\n\n"),VERSION);
		be_quiet = FALSE;
	}

	set_new_calcstate(curstate);
	set_new_errorout(puterror);
	set_new_infoout(puterror);
	
	if(!do_compile) {
		file = g_strconcat(LIBRARY_DIR,"/gel/lib.cgel",NULL);
		load_compiled_file(file,FALSE);
		g_free(file);

		/*try the library file in the current directory*/
		load_compiled_file("lib.cgel",FALSE);

		file = g_strconcat(getenv("HOME"),"/.geniusinit",NULL);
		if(file)
			load_file(file,FALSE);
		g_free(file);
	}

	if(files) {
		GList *t;
		do {
			fp = fopen(files->data,"r");
			push_file_info(files->data,1);
			t = files;
			files = g_list_remove_link(files,t);
			g_list_free_1(t);
			if(!fp) {
				pop_file_info();
				puterror(_("Can't open file"));
			}
		} while(!fp && files);
		if(!fp && !files)
			return 0;
	} else {
		fp = stdin;
		push_file_info(NULL,1);
	}
	if(inter && use_readline) {
		init_inter();
	}
	
	rl_event_hook = nop;

	for(;;) {
		for(;;) {
			if(inter && use_readline) /*use readline mode*/ {
				ETree *e;
				rewind_file_info();
				e = get_p_expression();
				if(e) evalexp_parsed(e,stdout,NULL,"= ",TRUE);
			} else {
				if(be_quiet)
					evalexp(NULL,fp,NULL,NULL,NULL,FALSE);
				else
					evalexp(NULL,fp,stdout,NULL,NULL,FALSE);
				if(interrupted)
					got_eof = TRUE;
			}

			if(got_eof) {
				if(inter)
					puts("");
				got_eof = FALSE;
				break;
			}
		}
		if(files) {
			GList *t;
			fclose(fp);
			do {
				fp = fopen(files->data,"r");
				push_file_info(files->data,1);
				t = files;
				files = g_list_remove_link(files,t);
				g_list_free_1(t);
				if(!fp) {
					pop_file_info();
					puterror(_("Can't open file"));
				}
			} while(!fp && files);
			if(!fp && !files) {
				if(do_compile) {
					push_file_info(NULL,0);
					compile_all_user_funcs(stdout);
					pop_file_info();
				}
				return 0;
			}
		} else
			break;
	}
	
	if(fp != stdin)
		fclose(fp);
	
	if(do_compile) {
		push_file_info(NULL,0);
		compile_all_user_funcs(stdout);
		pop_file_info();
	}

	return 0;
}
