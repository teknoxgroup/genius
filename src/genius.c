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

#include <glib.h>

#include <string.h>
#include <unistd.h>
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
	FALSE,
	FALSE
	};
	
extern calc_error_t error_num;
extern int got_eof;

static void
puterror(char *s)
{
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
		else if(strcmp(argv[i],"--floatstoints")==0)
			curstate.make_floats_ints = TRUE;
		else if(strcmp(argv[i],"--nofloatstoints")==0)
			curstate.make_floats_ints = FALSE;
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
		else if(strcmp(argv[i],"--help")==0) {
			puts("Genius "VERSION" usage:\n\n"
			     "genius [options] [files]\n\n"
			     "\t--precision=num   \tFloating point precision [256]\n"
			     "\t--maxdigits=num   \tMaximum digits to display (0=no limit) [0]\n"
			     "\t--[no]floatstoints\tConvert floats to integers in the\n"
			     "\t                  \tmiddle of calculations [OFF]\n"
			     "\t--[no]floatresult \tAll results as floats [OFF]\n"
			     "\t--[no]scinot      \tResults in scientific notation [OFF]\n"
			     "\t--[no]readline    \tUse readline even if it is available [ON]\n"
			     );
			exit(1);
		}
	}
	inter = isatty(0) && !files;
	/*interactive mode, print welcome message*/
	if(inter) {
		puts("Genius "VERSION"\n"
		     "Copyright (c) 1997,1998,1999 Free Software Foundation, Inc.\n"
		     "This is free software with ABSOLUTELY NO WARRANTY.\n"
		     "For details type `warranty'.\n");
	}
	
	file = g_strconcat(LIBRARY_DIR,"/lib.gel",NULL);
	if((fp = fopen(file,"r"))) {
		while(1) {
			g_free(evalexp(NULL,fp,NULL,NULL,curstate,puterror,FALSE));
			if(got_eof) {
				got_eof = FALSE;
				break;
			}
		}
		fclose(fp);
	}
	g_free(file);

	/*try the library file in the current directory*/
	if((fp = fopen("lib.gel","r"))) {
		while(1) {
			g_free(evalexp(NULL,fp,NULL,NULL,curstate,puterror,FALSE));
			if(got_eof) {
				got_eof = FALSE;
				break;
			}
		}
		fclose(fp);
	}
	
	file = g_strconcat(getenv("HOME"),"/.geniusinit",NULL);

	fp = NULL;
	if(file && (fp = fopen(file,"r"))) {
		while(1) {
			g_free(evalexp(NULL,fp,NULL,NULL,curstate,puterror,FALSE));
			if(got_eof) {
				got_eof = FALSE;
				break;
			}
		}
	}
	if(fp) fclose(fp);

	if(files) {
		GList *t;
		fp = fopen(files->data,"r");
		t = files;
		files = g_list_remove_link(files,t);
		g_list_free_1(t);
	} else
		fp = stdin;
	for(;;) {
		while(1/*!feof(fp)*/) {
#ifdef WITH_READLINE_SUPPORT
			if(inter && use_readline) /*use readline mode*/
				evalexp(NULL,NULL,stdout,"= ",curstate,puterror,TRUE);
			else
#endif
				evalexp(NULL,fp,stdout,NULL,curstate,puterror,FALSE);

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
			fp = fopen(files->data,"r");
			t = files;
			files = g_list_remove_link(files,t);
			g_list_free_1(t);
		} else
			break;
	}
	
	if(fp != stdin)
		fclose(fp);

	return 0;
}
