/* GnomENIUS Calculator
 * Copyright (C) 1997, 1998 the Free Software Foundation.
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
%{
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "mpwrap.h"
#include "eval.h"
#include "dict.h"
#include "calc.h"
#include "parse.h"
#include "util.h"
#ifdef WITH_READLINE_SUPPORT
#include <readline/readline.h>
#include <readline/history.h>
#endif

int lex_fd[2];
int first_tok = STARTTOK;
int return_ret = FALSE;
static int parenth_depth = 0;
int use_readline = FALSE;
int lex_init = TRUE;
extern evalstack_t evalstack;
extern int got_eof;
static int matrix_entry=0;

#define NO_RET {return_ret = FALSE;}
#define DO_RET { \
	if(!parenth_depth) \
		return_ret = TRUE; \
	else \
		return_ret = FALSE; \
}

#ifdef WITH_READLINE_SUPPORT
#define NEXT_LINE {							\
	while(use_readline) {						\
		char *s = readline(!evalstack?"genius> ":"      > ");	\
		if(!s) {						\
			got_eof = TRUE;					\
			return '\n';					\
		}							\
		if(!*s)	{						\
			free(s);					\
			continue;					\
		}							\
		add_history(s);						\
									\
		/*this seems ugly but for some reason that's the only	\
		  way it works*/					\
		close(lex_fd[0]);					\
		fclose(yyin);						\
									\
		pipe(lex_fd);						\
		yyin=fdopen(lex_fd[0],"r");				\
									\
		write(lex_fd[1],s,strlen(s));				\
		write(lex_fd[1],"\n",1);				\
		close(lex_fd[1]);					\
		free(s);						\
		break;							\
	}								\
}
#else
#define NEXT_LINE ;
#endif

%}

%%

%{
	if(lex_init) {
		return_ret = FALSE;
		parenth_depth = 0;
		lex_init = FALSE;
	}
	
	if(first_tok!=INT_MAX) {
		int i = first_tok;
		first_tok = INT_MAX;
		return i;
	}
%}

#.*$		; /*comment, ignore*/

^[ 	]*load[ 	]+\"([^"]|\\\")*\"[ 	]*$ {
			char *s;
			char *end;
			if(evalstack || parenth_depth) {REJECT;}
			s=strchr(yytext,'"')+1; /*find the beginning*/
			end=strrchr(yytext,'"'); /*find the end*/
			yylval.id = unescape_string(s,end,NULL);
			DO_RET;
			return LOADFILE;
		}

^[ 	]*load[ 	]+\'([^']|\\\')*\'[ 	]*$ {
			char *s;
			char *end;
			if(evalstack || parenth_depth) {REJECT;}
			s=strchr(yytext,'\'')+1; /*find the beginning*/
			end=strrchr(yytext,'\''); /*find the end*/
			yylval.id = g_strndup(s,end-s);
			DO_RET;
			return LOADFILE;
		}

^[ 	]*load[ 	]+[^ 	].*$ {
			char *s;
			char *end;
			if(evalstack || parenth_depth) {REJECT;}
			s=strchr(yytext,'d')+1; /*move after the load*/
			/*btw from the above RE we know this will work*/
			while(*s==' ' || *s=='\t')
				s++;
			yylval.id = g_strdup(s);
			DO_RET;
			return LOADFILE_GLOB;
		}

";"		{ DO_RET; return SEPAR; }

\"([^"]|\\\")*\"	{ 
			DO_RET;
			yylval.id = unescape_string(&yytext[1],NULL,NULL);
			/*kill the trailing "*/
			yylval.id[strlen(yylval.id)-1]='\0';
			return STRING;
		}

==		{ NO_RET; return EQ_CMP; }
!=		{ NO_RET; return NE_CMP; }
"<=>"		{ NO_RET; return CMP_CMP; }

"<="		{ NO_RET; return LE_CMP; }
">="		{ NO_RET; return GE_CMP; }

"<"		{ NO_RET; return LT_CMP; }
">"		{ NO_RET; return GT_CMP; }

=		{ NO_RET; return EQUALS; }

not		{ NO_RET; return LOGICAL_NOT; }
and		{ NO_RET; return LOGICAL_AND; }
xor		{ NO_RET; return LOGICAL_XOR; }
or		{ NO_RET; return LOGICAL_OR; }

while		{ NO_RET; return WHILE; }
do		{ NO_RET; return DO; }
until		{ NO_RET; return UNTIL; }
if		{ NO_RET; return IF; }
then		{ NO_RET; return THEN; }
else		{ NO_RET; return ELSE; }

define		{ NO_RET; return DEFINE; }
call		{ NO_RET; return CALL; }

return		{ NO_RET; return RETURNTOK; }
bailout		{ DO_RET; return BAILOUT; }
exception	{ DO_RET; return EXCEPTION; }
continue	{ DO_RET; return CONTINUE; }
break		{ DO_RET; return BREAK; }

"@("		{
			parenth_depth++;
			NO_RET;
			return AT;
		}
\.\.		{
			REJECT;
			/*FIXME:*/
			/*NO_RET;
			return REGION_SEP;*/
		}
		
[[][\t ]*	{
			matrix_entry++;
			parenth_depth++;
			NO_RET;
			return yytext[0];
		}

[\t ]*[]]	{
			matrix_entry--;
			parenth_depth--;
			DO_RET;
			return yytext[0];
		}
			

[({]		{
			parenth_depth++;
			NO_RET;
			return yytext[0];
		}

[)}]		{
			parenth_depth--;
			DO_RET;
			return yytext[0];
		}

[a-zA-Z_][a-zA-Z0-9_]*	{
				/* identifier*/
				yylval.id = g_strdup(yytext);
				DO_RET;
				return FUNCID;
			}

[0-9]*\.[0-9]+[e@]-?[0-9]+i	|
[0-9]+[e@]-?[0-9]+i		|
[0-9]+i				|
[0-9]*\.[0-9]+i	{
			char *s;
			if(yytext[0] == '.')
				s = g_strconcat("0",&yytext[0],NULL);
			else
				s = g_strdup(yytext);
			mpw_init(yylval.val);
			mpw_set_str_complex(yylval.val,s,10);
			g_free(s);
			DO_RET;
			return NUMBER;
		}


[0-9]*\.[0-9]+[e@]-?[0-9]+	|
[0-9]+[e@]-?[0-9]+		|
[0-9]*\.[0-9]+	{
			char *s;
			if(yytext[0] == '.')
				s = g_strconcat("0",&yytext[0],NULL);
			else
				s = g_strdup(yytext);
			mpw_init(yylval.val);
			mpw_set_str_float(yylval.val,s,10);
			g_free(s);
			DO_RET;
			return NUMBER;
		}


([1-3][0-9]|[1-9])\\[0-9a-zA-Z]+	{
			char *s;
			int base;

			s=g_malloc(strlen(yytext)-2); /*minus 3 and plus 1*/
			sscanf(yytext,"%d\\%[0-9a-zA-Z]s",&base,s);
			if(base>36) { g_free(s); REJECT; }
			mpw_init(yylval.val);
			mpw_set_str_int(yylval.val,s,base);
			g_free(s);
			DO_RET;
			return NUMBER;
		}

0x[0-9a-fA-F]+	|
0[0-7]+		|
[0-9]+		{
			mpw_init(yylval.val);
			mpw_set_str_int(yylval.val,yytext,0);
			DO_RET;
			return NUMBER;
		}
		
\\\n		{ NEXT_LINE; }
		
[\t ]*\n	{ 
			if(matrix_entry) {
				NEXT_LINE;
				return ':';
			} else if(return_ret) {
				first_tok = 0;
				return '\n';
			}
			NEXT_LINE;
		}
		
^[\t ][\t ]*	;

[\t ]*,[\t ]*	{ return ','; }

[\t][\t ]*	{
			if(matrix_entry)
				return ',';
		}

[ \t\r]+	; /*ignore whitespace*/

[!']		{
			DO_RET;
			return yytext[0];
		}

[@^*/%+-]	{ NO_RET; return yytext[0]; }

\.		{ DO_RET; return '.'; }

\|		{ DO_RET; return '|'; }

<<EOF>>		{ got_eof = TRUE; return '\n'; }

.		{ NO_RET; return yytext[0]; }

%%
