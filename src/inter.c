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

#include "config.h"

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
#include "calc.h"
#include "plugin.h"

#include "inter.h"

extern int interrupted;
extern int got_eof;

static int toplevelokg = TRUE;

static int
ok_for_top(char *s)
{
	char *t = g_strstrip(g_strdup(s));
	if(strncmp(t,"plugin",strlen(t))==0 ||
	   strncmp(t,"load",strlen(t))==0) {
		g_free(t);
		return TRUE;
	} else {
		g_free(t);
		return FALSE;
	}
}

ETree *
get_p_expression(void)
{
	GString *gs;
	char *prompt = "genius> ";
	
	interrupted = FALSE;
	
	gs = g_string_new("");
	
	for(;;) {
		int finished;
		char *s;
		ETree *ret;
		int oldtop = toplevelokg;

		toplevelokg = ok_for_top(gs->str);
		s = readline(prompt);
		toplevelokg = oldtop;
		
		if(interrupted) {
			g_string_free(gs,TRUE);
			if(s) free(s);
			return NULL;
		}

		prompt = "      > ";
		if(!s) {
			got_eof = TRUE;
			g_string_append_c(gs,'\n');
			ret = parseexp(gs->str,NULL,TRUE,FALSE,NULL);
			g_string_free(gs,TRUE);
			return ret;
		}
		if(!*s)	{
			free(s);
			continue;
		}
		add_history(s);
		g_string_append(gs,s);
		free(s);
		g_string_append_c(gs,'\n');
		
		ret = parseexp(gs->str,NULL,TRUE,TRUE,&finished);
		if(got_eof)
			got_eof = FALSE;
		if(finished) {
			g_string_free(gs,TRUE);
			return ret;
		}
	}
}

static int addtoplevels = TRUE;
static char *toplevels[] = {
	"load",
	"plugin",
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

static char *
plugin_generator (char *text, int state)
{
	static int len;
	static GList *li;

	if(!state) {
		len = strlen (text);
		li = plugin_list;
	}

	while(li) {
		plugin_t *plg = li->data;
		li = g_list_next(li);
		if(!plg->base)
			continue;
		if(strncmp(plg->base,text,len)==0)
			return strdup(plg->base);
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
	   (strncmp(p,"plugin ",7)==0 ||
	    strncmp(p,"plugin\t",7)==0)) {
		return completion_matches (text, plugin_generator);
	}
	
	
	if(toplevelokg &&
	   (!*p || strncmp(p,"load",strlen(p))==0 ||
	    strncmp(p,"plugin",strlen(p))==0))
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
