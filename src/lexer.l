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
#include <string.h>
#include "mpwrap.h"
#include "eval.h"
#include "dict.h"
#include "calc.h"
#include "parse.h"
#ifdef WITH_READLINE_SUPPORT
#include <readline/readline.h>
#endif

int lex_fd[2];
int first_tok = INFIX_EXPR;
int return_ret = FALSE;
static int block_depth = 0;
static int parenth_depth = 0;
GList *notation_stack = NULL;
int use_readline = FALSE;
int lex_init = TRUE;
extern evalstack_t evalstack;

void push_notation(notation_t not);
void pop_notation(void);

#define LEX_NOTATION (GPOINTER_TO_INT(notation_stack->data))

#define NO_RET {return_ret = FALSE;}
#define DO_RET { \
	if(!block_depth && !parenth_depth) \
		return_ret = TRUE; \
	else \
		return_ret = FALSE; \
}

#define FUNC_RET { \
	if(LEX_NOTATION==POSTFIX_NOTATION) { \
		DO_RET; \
	} else { \
		NO_RET; \
	} \
}

#ifdef WITH_READLINE_SUPPORT
#define NEXT_LINE {							\
	while(use_readline) {						\
		char *s = readline(!evalstack?"genius> ":"> ");		\
		if(!s)							\
			return EOF;					\
		if(!*s)							\
			continue;					\
		add_history(s);						\
									\
		/*this seems ugly but for some reason that's the only	\
		  way it works*/					\
		close(lex_fd[0]);					\
									\
		pipe(lex_fd);						\
		yyin=fdopen(lex_fd[0],"r");				\
									\
		write(lex_fd[1],s,strlen(s));				\
		write(lex_fd[1],"\n",1);				\
		close(lex_fd[1]);					\
		break;							\
	}								\
}
#else
#define NEXT_LINE ;
#endif

%}

%s NEGATIVE

%%

%{
	if(lex_init) {
		return_ret = FALSE;
		block_depth = 0;
		parenth_depth = 0;
		lex_init = FALSE;
	}
	
	if(first_tok!=INT_MAX) {
		int i = first_tok;
		first_tok = INT_MAX;
		if(i==INFIX_EXPR) {
			BEGIN INITIAL;
		} else {
			BEGIN NEGATIVE;
		}
		return i;
	}
%}

#.*$		; /*comment, ignore*/

infix		{
			NO_RET;
			push_notation(INFIX_NOTATION);
			return INLINE_INFIX;
		}
prefix		{
			NO_RET;
			push_notation(PREFIX_NOTATION);
			return INLINE_PREFIX;
		}
postfix		{
			NO_RET;
			push_notation(POSTFIX_NOTATION);
			return INLINE_POSTFIX;
		}

\;		{ NO_RET; return SEPAR; }

~		{ NO_RET; return NEG; }

==		{ NO_RET; return EQ_CMP; }
!=		{ NO_RET; return NE_CMP; }
\<=\>		{ NO_RET; return CMP_CMP; }

\<=		{ NO_RET; return LE_CMP; }
\>=		{ NO_RET; return GE_CMP; }

\<		{ NO_RET; return LT_CMP; }
\>		{ NO_RET; return GT_CMP; }

=		{ NO_RET; return EQUALS; }

not		{ NO_RET; return LOGICAL_NOT; }
and		{ NO_RET; return LOGICAL_AND; }
xor		{ NO_RET; return LOGICAL_XOR; }
or		{ NO_RET; return LOGICAL_OR; }

while		{ NO_RET; return WHILE_CONS; }

ifelse		{ NO_RET; return IFELSE_CONS; }

if		{ NO_RET; return IF_CONS; }

\{		{
			/*create a new context and push it on top of the
			  context stack, it's only temporary*/
			d_addcontext(NULL);
			block_depth++;
			NO_RET;
			return BEGINBLOCK;
		}

\}		{

			/*kill the temporary context, but keep the
			  declarations*/
			freedict(d_popcontext(),TRUE);
			block_depth--;
			DO_RET;
			return ENDBLOCK;
		}
		
[[(]		{
			parenth_depth++;
			NO_RET;
			return yytext[0];
		}

[])]		{
			parenth_depth--;
			DO_RET;
			return yytext[0];
		}

arg[1-8]	{
			DO_RET;
			/*make static argument placeholders*/
			if((yylval.func=d_lookup(yytext,FALSE))==NULL)
				yylval.func=d_addfunc(d_makeufunc(yytext,
					NULL,NULL,0,FALSE));
			return FUNCID0;
		}

[0-8]\\[a-zA-Z][a-zA-Z0-9]*	{
				/*this is the function declaration*/
				NO_RET;

				/*create a dummy function, first character is
				  the number of arguments*/
				yylval.func=
					d_addfunc(d_makeufunc(&yytext[2],NULL,
						NULL,(int)(yytext[0]-'0'),
						FALSE));
				return FUNCDEF;
			}

[a-zA-Z][a-zA-Z0-9]*	{
				/* we need to find out the number of arguments
				   for this function, this is a limitation of
				   mostly the postfix and prefix notation
				   without parenthesis */
				func_t *n;

				n=d_lookup(yytext,TRUE);
				if(!n) {
					/*create a dummy function*/
					yylval.func=
						d_addfunc(d_makeufunc(
							yytext,NULL,NULL,0,
							FALSE));
					DO_RET;
					return FUNCID0;
				}

				yylval.func=n;
				switch(n->nargs) {
					case 0: DO_RET; return FUNCID0;
					case 1: FUNC_RET; return FUNCID1;
					case 2: FUNC_RET; return FUNCID2;
					case 3: FUNC_RET; return FUNCID3;
					case 4: FUNC_RET; return FUNCID4;
					case 5: FUNC_RET; return FUNCID5;
					case 6: FUNC_RET; return FUNCID6;
					case 7: FUNC_RET; return FUNCID7;
					case 8: FUNC_RET; return FUNCID8;
				}
				/*too many args or corrupted entry, this
				  shouldn't happen, and this is not a way
				  to deal with it, but oh well, for now*/
				REJECT;
			}

<NEGATIVE>-[0-9]*\.[0-9]+[e@]-?[0-9]+i	|
<NEGATIVE>-[0-9]+[e@]-?[0-9]+i		|
<NEGATIVE>-[0-9]+i			|
<NEGATIVE>-[0-9]*\.[0-9]+i	{
			char *s;
			if(yytext[1] == '.')
				s = g_strconcat("-0",&yytext[1],NULL);
			else
				s = g_strdup(yytext);
			mpw_init(yylval.val);
			mpw_set_str_complex(yylval.val,s,10);
			g_free(s);
			DO_RET;
			return NEGNUM;
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
			return POSNUM;
		}

<NEGATIVE>-[0-9]*\.[0-9]+[e@]-?[0-9]+	|
<NEGATIVE>-[0-9]+[e@]-?[0-9]+		|
<NEGATIVE>-[0-9]*\.[0-9]+	{
			char *s;
			if(yytext[1] == '.')
				s = g_strconcat("-0",&yytext[1],NULL);
			else
				s = g_strdup(yytext);
			mpw_init(yylval.val);
			mpw_set_str_float(yylval.val,s,10);
			g_free(s);
			DO_RET;
			return NEGNUM;
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
			return POSNUM;
		}

<NEGATIVE>-([1-3][0-9]|[1-9])#[0-9a-zA-Z]+#	{
			char *s;
			int base;

			s=g_malloc(strlen(yytext)-2); /*minus 3 and plus 1*/
			sscanf(yytext,"-%d#%[0-9a-zA-Z]s#",&base,s);
			if(base>36) { g_free(s); REJECT; }
			mpw_init(yylval.val);
			mpw_set_str_int(yylval.val,s,base);
			mpw_neg(yylval.val,yylval.val);
			DO_RET;
			return NEGNUM;
		}

([1-3][0-9]|[1-9])#[0-9a-zA-Z]+#	{
			char *s;
			int base;

			s=g_malloc(strlen(yytext)-2); /*minus 3 and plus 1*/
			sscanf(yytext,"%d#%[0-9a-zA-Z]s#",&base,s);
			if(base>36) { g_free(s); REJECT; }
			mpw_init(yylval.val);
			mpw_set_str_int(yylval.val,s,base);
			DO_RET;
			return POSNUM;
		}

<NEGATIVE>-0x[0-9a-fA-F]+	|
<NEGATIVE>-0[0-7]+	|
<NEGATIVE>-[0-9]+		{
			mpw_init(yylval.val);
			mpw_set_str_int(yylval.val,yytext,0);
			DO_RET;
			return NEGNUM;
		}

0x[0-9a-fA-F]+	|
0[0-7]+		|
[0-9]+		{
			mpw_init(yylval.val);
			mpw_set_str_int(yylval.val,yytext,0);
			DO_RET;
			return POSNUM;
		}
		
\\\n		{ NEXT_LINE; }
		
\n		{ 
			if(return_ret) {
				first_tok = 0;
				return '\n';
			}
			NEXT_LINE;
		}
[ \t\r]+	; /*ignore whitespace*/

\!		{
			if(LEX_NOTATION==PREFIX_NOTATION) {
				NO_RET;
			} else {
				DO_RET;
			}
			return yytext[0];
		}

[@^*/%+-]	{ FUNC_RET; return yytext[0]; }

\.		{ DO_RET; return '.'; }

.		{ NO_RET; return yytext[0]; }

%%

void
push_notation(notation_t not)
{
	stack_push(&notation_stack,GINT_TO_POINTER(not));
	if(not==INFIX_NOTATION) {
		BEGIN INITIAL;
	} else {
		BEGIN NEGATIVE;
	}
}

void
pop_notation(void)
{
	notation_t not;
	stack_pop(&notation_stack);

	not = GPOINTER_TO_INT(notation_stack->data);
	if(not==INFIX_NOTATION) {
		BEGIN INITIAL;
	} else {
		BEGIN NEGATIVE;
	}
}

