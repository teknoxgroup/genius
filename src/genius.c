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

#ifdef GNOME_SUPPORT
#include <gnome.h>
#else
#define _(x) x
#endif

#include "config.h"

#include <glib.h>

#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "calc.h"
#include "util.h"

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

int
main(int argc, char *argv[])
{
	int i;
	int inter;
	int use_readline = TRUE;
	int lastarg = FALSE;
	GList *files = NULL;
	char *file;
	FILE *fp;
	int do_compile = FALSE;
	int be_quiet = FALSE;

#ifdef GNOME_SUPPORT
	bindtextdomain(PACKAGE,GNOMELOCALEDIR);
	textdomain(PACKAGE);
#endif

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
			puts("Genius "VERSION" usage:\n\n"
			     "genius [options] [files]\n\n"
			     "\t--precision=num   \tFloating point precision [256]\n"
			     "\t--maxdigits=num   \tMaximum digits to display (0=no limit) [0]\n"
			     "\t--[no]floatresult \tAll results as floats [OFF]\n"
			     "\t--[no]scinot      \tResults in scientific notation [OFF]\n"
			     "\t--[no]readline    \tUse readline even if it is available [ON]\n"
			     "\t--[no]compile     \tCompile everything and dump it to stdout [OFF]\n"
			     "\t--[no]quiet       \tBe quiet during non-interactive mode,\n"
			     "\t                  \t(always on when compiling) [OFF]\n"
			     );
			exit(1);
		}
	}
	if(do_compile)
		be_quiet = TRUE;
	inter = isatty(0) && !files && !do_compile;
	/*interactive mode, print welcome message*/
	if(inter) {
		puts(_("Genius "VERSION"\n"
		     "Copyright (c) 1997,1998,1999 Free Software Foundation, Inc.\n"
		     "This is free software with ABSOLUTELY NO WARRANTY.\n"
		     "For details type `warranty'.\n"));
		signal(SIGINT,SIG_IGN);
		curstate.do_interrupts = TRUE;
		be_quiet = FALSE;
	} else
		curstate.do_interrupts = FALSE;
	
	if(!do_compile) {
		file = g_strconcat(LIBRARY_DIR,"/lib.cgel",NULL);
		load_compiled_file(file,curstate,puterror,FALSE);
		g_free(file);

		/*try the library file in the current directory*/
		load_compiled_file("lib.cgel",curstate,puterror,FALSE);

		file = g_strconcat(getenv("HOME"),"/.geniusinit",NULL);
		if(file)
			load_file(file,curstate,puterror,FALSE);
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
	for(;;) {
		while(1/*!feof(fp)*/) {
#ifdef WITH_READLINE_SUPPORT
			if(inter && use_readline) /*use readline mode*/ {
				rewind_file_info();
				evalexp(NULL,NULL,stdout,"= ",curstate,puterror,TRUE);
			} else {
#endif
				if(be_quiet)
					g_free(evalexp(NULL,fp,NULL,NULL,curstate,puterror,FALSE));
				else
					evalexp(NULL,fp,stdout,NULL,curstate,puterror,FALSE);
#ifdef WITH_READLINE_SUPPORT
			}
#endif

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
					compile_all_user_funcs(stdout,puterror);
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
		compile_all_user_funcs(stdout,puterror);
		pop_file_info();
	}

	return 0;
}
