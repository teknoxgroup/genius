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

#ifdef GNOME_SUPPORT
#include <gnome.h>
#else
#include <libintl.h>
#define _(x) gettext(x)
#endif

#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "dict.h"

#include "inter.h"

extern int interrupted;

static char *startw[]={
	"not","and","xor","or","while","until","for","do","to","by","in","if",
	"then","else","define","function","call","return",NULL
};

static int toplevelokg = TRUE;
	

/*SORT of an ugly hack, this has to match the lexer/parser at figuring
  out what should be processed as a single expression, hopefully the two
  match, all this hooplah is because we can only have one parser/lexer
  active at a single time due to the braindamage of lex/yacc*/
char *
get_expression(int *ieof)
{
	GString *gs;
	char *r;
	int pardepth = 0;
	int toplevelok = TRUE;
	int do_ret = TRUE;
	char *prompt = "genius> ";
	
	interrupted = FALSE;
	
	gs = g_string_new("");
	
	for(;;) {
		char *p;
		char *s;
		int i;
		int found;
		int oldtop = toplevelokg;

		toplevelokg = toplevelok;
		s = readline(prompt);
		toplevelokg = oldtop;
		
		if(interrupted) {
			g_string_free(gs,TRUE);
			if(s) free(s);
			return NULL;
		}

		prompt = "      > ";
		if(!s) {
			*ieof = TRUE;
			g_string_append_c(gs,'\n');
			break;
		}
		if(!*s)	{
			free(s);
			continue;
		}
		add_history(s);
		g_string_append(gs,s);
		g_string_append_c(gs,'\n');
		
		if(toplevelok) {
			char *ss = g_strstrip(g_strdup(s));
			
			if(strncmp(ss,"load",4)==0 &&
			   (ss[4]==' ' || ss[4]=='\t')) {
				for(p=&ss[4];*p;p++) {
					if(*p!=' ' && *p!='\t') {
						free(s);
						g_free(ss);
						r = gs->str;
						g_string_free(gs,FALSE);
						return r;
					}
				}
			}
			g_free(ss);
		}
		for(p=s;*p;p++) {
			switch(*p) {
			case ' ':
			case '\t':
				break;
			case '(':
			case '[':
			case '{':
				pardepth++;
				break;
			case ')':
			case ']':
			case '}':
				pardepth--;
				break;
			default:
				toplevelok=FALSE;
				break;
			}
		}
		if(pardepth>0) {
			free(s);
			continue;
		}
		p--;
		if(*p=='\\') {
			free(s);
			continue;
		}
		while(p>=s && isspace(*p)) p--;
		
		if(p<s || *p=='=' || *p=='>' || *p=='<' ||
		   (p>s && *p=='.' && *(p-1)=='.') ||
		   *p=='@' || *p=='^' || *p=='*' || *p=='/' ||
		   *p=='%' || *p=='+' || *p=='-') {
			free(s);
			continue;
		}
		
		/*NOTICE: HERE we modify s*/
		*(p+1)='\0';

		while(p>=s && (isalnum(*p) || *p=='_')) p--;
		p++;
		while(*p && isdigit(*p)) p++;
		
		/*could be a complex number before identifier*/
		if(*p=='i') p++;

		found = FALSE;
		for(i=0;startw[i];i++) {
			if(strcmp(p,startw[i])==0) {
				found = TRUE;
				break;
			}
		}
		if(found) {
			free(s);
			continue;
		}

		free(s);
		break;
	}
	
	r = gs->str;
	g_string_free(gs,FALSE);
	return r;
}

static int addtoplevels = TRUE;
static char *toplevels[] = {
	"load",
	NULL
};
static char *operators[] = {
	"not","and","xor","or","while","until","for","do","to","by","in","if",
	"then","else","define","function","call","return","bailout","exception",
	"continue","break","null",
	NULL
};


static char *
command_generator (char *text, int state)
{
	static int oi,ti,len;
	static GList *fli;

	if(!state) {
		oi = 0;
		if(addtoplevels)
			ti = 0;
		else
			ti = -1;
		len = strlen (text);
		fli = d_getcontext();
	}
	
	while(ti>=0 && toplevels[ti]) {
		char *s = toplevels[ti++];
		if(strncmp(s,text,len)==0)
			return strdup(s);
	}

	while(operators[oi]) {
		char *s = operators[oi++];
		if(strncmp(s,text,len)==0)
			return strdup(s);
	}

	while(fli) {
		EFunc *f = fli->data;
		fli = g_list_next(fli);
		if(!f->id || !f->id->token)
			continue;
		if(strncmp(f->id->token,text,len)==0)
			return strdup(f->id->token);
	}

	return NULL;
}

static char **
tab_completion (char *text, int start, int end)
{
	char *p;
	for(p=rl_line_buffer;*p==' ' || *p=='\t';p++)
		;
	if(toplevelokg &&
	   (strncmp(p,"load ",5)==0 ||
	    strncmp(p,"load\t",5)==0)) {
		return NULL;
	}
	
	if(toplevelokg &&
	   (!*p || strncmp(p,"load",strlen(p))==0))
		addtoplevels = TRUE;
	else
		addtoplevels = FALSE;

	return completion_matches (text, command_generator);
}

void
init_inter(void)
{
	rl_readline_name = "Genius";
	rl_attempted_completion_function =
		(CPPFunction *)tab_completion;
}
