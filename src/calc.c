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
#define _(x) x
#endif
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <glob.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <glib.h>
#ifdef WITH_READLINE_SUPPORT
#include <readline/readline.h>
#include <readline/history.h>
#endif

#include "calc.h"
#include "eval.h"
#include "util.h"
#include "dict.h"
#include "funclib.h"
#include "matrixw.h"

#include "mpwrap.h"

#include "parse.h"

extern int lex_fd[2];
extern int first_tok;
extern int lex_init;
extern int use_readline;

extern FILE *yyin;
extern char *yytext;
extern int yydebug;

extern ETree *returnval;
extern int inbailout;
extern int inexception;

extern GSList *inloop;
extern char *loadfile;
extern char *loadfile_glob;

/* stack ... has to be global:-( */
evalstack_t evalstack=NULL;

/*error .. global as well*/
calc_error_t error_num = NO_ERROR;
int got_eof = FALSE;

/*the current state of the calculator*/
calcstate_t calcstate;

/*error reporting function*/
void (*errorout)(char *)=NULL;

char *loadfile = NULL;
char *loadfile_glob = NULL;

int interrupted = FALSE;

static void
appendout_c(GString *gs, FILE *out, char c)
{
	if(!out)
		g_string_append_c(gs,c);
	else
		fputc(c,out);
}

static void
appendout(GString *gs, FILE *out, char *s)
{
	if(!out)
		g_string_append(gs,s);
	else
		fputs(s,out);
}

static void
append_binaryoper(GString *gs,FILE *out,char *p, ETree *n)
{
	ETree *l,*r;
	GET_LR(n,l,r);
	appendout_c(gs,out,'(');
	print_etree(gs,out,l);
	appendout(gs,out,p);
	print_etree(gs,out,r);
	appendout_c(gs,out,')');
}

static void
append_unaryoper(GString *gs,FILE *out,char *p, ETree *n)
{
	ETree *l;
	GET_L(n,l);
	appendout_c(gs,out,'(');
	appendout(gs,out,p);
	print_etree(gs,out,l);
	appendout_c(gs,out,')');
}

static void
appendoper(GString *gs,FILE *out, ETree *n)
{
	ETree *l,*r,*rr;
	GList *li;

	switch(n->data.oper) {
		case E_SEPAR:
			append_binaryoper(gs,out,";",n); break;
		case E_EQUALS:
			append_binaryoper(gs,out,"=",n); break;
		case E_ABS:
			GET_L(n,l);
			appendout(gs,out,"(|");
			print_etree(gs,out,l);
			appendout(gs,out,"|)");
			break;
		case E_PLUS:
			append_binaryoper(gs,out,"+",n); break;
		case E_MINUS:
			append_binaryoper(gs,out,"-",n); break;
		case E_MUL:
			append_binaryoper(gs,out,"*",n); break;
		case E_DIV:
			append_binaryoper(gs,out,"/",n); break;
		case E_MOD:
			append_binaryoper(gs,out,"%",n); break;
		case E_NEG:
			append_unaryoper(gs,out,"-",n); break;
		case E_EXP:
			append_binaryoper(gs,out,"^",n); break;
		case E_FACT:
			GET_L(n,l);
			appendout_c(gs,out,'(');
			print_etree(gs,out,l);
			appendout_c(gs,out,'!');
			appendout_c(gs,out,')');
			break;
		case E_TRANSPOSE:
			GET_L(n,l);
			appendout_c(gs,out,'(');
			print_etree(gs,out,l);
			appendout_c(gs,out,'\'');
			appendout_c(gs,out,')');
			break;

		case E_EQ_CMP:
			append_binaryoper(gs,out,"==",n); break;
		case E_NE_CMP:
			append_binaryoper(gs,out,"!=",n); break;
		case E_CMP_CMP:
			append_binaryoper(gs,out,"<=>",n); break;
		case E_LT_CMP:
			append_binaryoper(gs,out,"<",n); break;
		case E_GT_CMP:
			append_binaryoper(gs,out,">",n); break;
		case E_LE_CMP:
			append_binaryoper(gs,out,"<=",n); break;
		case E_GE_CMP:
			append_binaryoper(gs,out,">=",n); break;
		case E_LOGICAL_AND:
			append_binaryoper(gs,out," and ",n); break;
		case E_LOGICAL_OR:
			append_binaryoper(gs,out," or ",n); break;
		case E_LOGICAL_XOR:
			append_binaryoper(gs,out," xor ",n); break;
		case E_LOGICAL_NOT:
			append_unaryoper(gs,out,"not ",n); break;

		case E_REGION_SEP:
			append_binaryoper(gs,out,"..",n); break;

		case E_GET_ELEMENT:
		case E_GET_REGION:
			GET_LRR(n,l,r,rr);
			appendout_c(gs,out,'(');
			print_etree(gs,out,l);
			appendout(gs,out,"@(");
			print_etree(gs,out,r);
			appendout_c(gs,out,',');
			print_etree(gs,out,rr);
			appendout(gs,out,"))");
			break;
		case E_GET_ROW_REGION:
			GET_LR(n,l,r);
			appendout_c(gs,out,'(');
			print_etree(gs,out,l);
			appendout(gs,out,"@[");
			print_etree(gs,out,r);
			appendout(gs,out,",])");
			break;
		case E_GET_COL_REGION:
			GET_LR(n,l,r);
			appendout_c(gs,out,'(');
			print_etree(gs,out,l);
			appendout(gs,out,"@[,");
			print_etree(gs,out,r);
			appendout(gs,out,"])");
			break;

		case E_REFERENCE:
			append_unaryoper(gs,out,"&",n); break;
		case E_DEREFERENCE:
			append_unaryoper(gs,out,"*",n); break;

		case E_IF_CONS:
			GET_LR(n,l,r);
			appendout(gs,out,"(if ");
			print_etree(gs,out,l);
			appendout(gs,out," then ");
			print_etree(gs,out,r);
			appendout(gs,out,")");
			break;
		case E_IFELSE_CONS:
			GET_LRR(n,l,r,rr);
			appendout(gs,out,"(if ");
			print_etree(gs,out,l);
			appendout(gs,out," then ");
			print_etree(gs,out,r);
			appendout(gs,out," else ");
			print_etree(gs,out,rr);
			appendout(gs,out,")");
			break;
		case E_WHILE_CONS:
			GET_LR(n,l,r);
			appendout(gs,out,"(while ");
			print_etree(gs,out,l);
			appendout(gs,out," do ");
			print_etree(gs,out,r);
			appendout(gs,out,")");
			break;
		case E_UNTIL_CONS:
			GET_LR(n,l,r);
			appendout(gs,out,"(until ");
			print_etree(gs,out,l);
			appendout(gs,out," do ");
			print_etree(gs,out,r);
			appendout(gs,out,")");
			break;
		case E_DOWHILE_CONS:
			GET_LR(n,l,r);
			appendout(gs,out,"(do ");
			print_etree(gs,out,l);
			appendout(gs,out," while ");
			print_etree(gs,out,r);
			appendout(gs,out,")");
			break;
		case E_DOUNTIL_CONS:
			GET_LR(n,l,r);
			appendout(gs,out,"(do ");
			print_etree(gs,out,l);
			appendout(gs,out," until ");
			print_etree(gs,out,r);
			appendout(gs,out,")");
			break;

		case E_DIRECTCALL:
		case E_CALL:
			GET_L(n,l);
			appendout_c(gs,out,'(');
			if(l->type==IDENTIFIER_NODE) {
				appendout(gs,out,l->data.id->token);
			} else if(l->type == OPERATOR_NODE && l->data.oper == E_DEREFERENCE) {
				ETree *t;
				GET_L(l,t);
				if(t->type!=IDENTIFIER_NODE) {
					(*errorout)(_("Bad identifier for function node!"));
					appendout(gs,out,"???)");
					break;
				}
				appendout_c(gs,out,'*');
				appendout(gs,out,t->data.id->token);
			} else {
				(*errorout)(_("Bad identifier for function node!"));
				appendout(gs,out,"???)");
				break;
			}
			appendout_c(gs,out,'(');
			li = n->args->next;
			print_etree(gs,out,li->data);
			li=g_list_next(li);
			for(;li!=NULL;li=g_list_next(li)) {
				appendout_c(gs,out,',');
				print_etree(gs,out,li->data);
			}
			appendout(gs,out,"))");
			break;
		case E_RETURN:
			append_unaryoper(gs,out,"return ",n); break;
		case E_BAILOUT:
			appendout(gs,out,"(bailout)"); break;
		case E_EXCEPTION:
			appendout(gs,out,"(exception)"); break;
		case E_CONTINUE:
			appendout(gs,out,"(continue)"); break;
		case E_BREAK:
			appendout(gs,out,"(break)"); break;

		default:
			(*errorout)(_("Unexpected operator!"));
			appendout(gs,out,"(???)");
		       break;
	}
}

static void
appendmatrix(GString *gs, FILE *out, MatrixW *m)
{
	int i,j;
	appendout(gs,out,"[");
	
	print_etree(gs,out,matrixw_index(m,0,0));
	
	for(i=1;i<matrixw_width(m);i++) {
		appendout(gs,out,",");
		print_etree(gs,out,matrixw_index(m,i,0));
	}
	for(j=1;j<matrixw_height(m);j++) {
		appendout(gs,out,":");
		print_etree(gs,out,matrixw_index(m,0,j));
		for(i=1;i<matrixw_width(m);i++) {
			appendout(gs,out,",");
			print_etree(gs,out,matrixw_index(m,i,j));
		}
	}

	appendout(gs,out,"]");
}


/*make a string representation of an expression*/
void
print_etree(GString *gs, FILE *out, ETree *n)
{
	char *p;
	
	if(!n) {
		(*errorout)(_("NULL tree!"));
		appendout(gs,out,"(???)");
		return;
	}

	switch(n->type) {
	case NULL_NODE:
		appendout(gs,out,"(null)");
		break;
	case VALUE_NODE:
		p=mpw_getstring(n->data.value,calcstate.max_digits,
				calcstate.scientific_notation,
				calcstate.results_as_floats);
		appendout(gs,out,p);
		g_free(p);
		break;
	case MATRIX_NODE:
		appendmatrix(gs,out,n->data.matrix);
		break;
	case OPERATOR_NODE:
		appendoper(gs,out,n);
		break;
	case IDENTIFIER_NODE:
		appendout(gs,out,n->data.id->token);
		break;
	case STRING_NODE:
		appendout_c(gs,out,'"');
		p = escape_string(n->data.str);
		appendout(gs,out,p);
		g_free(p);
		appendout_c(gs,out,'"');
		break;
	case FUNCTION_NODE:
		{
			GList *li;
			EFunc *f;
			
			f = n->data.func;
			if(!f) {
				(*errorout)(_("NULL function!"));
				appendout(gs,out,"(???)");
				break;
			}

			appendout(gs,out,"(`(");

			for(li=f->named_args; li!=NULL; li=g_list_next(li)) {
				Token *id = li->data;
				if(li!=f->named_args)
					appendout_c(gs,out,',');
				appendout(gs,out,id->token);
			}

			if(f->type==USER_FUNC) {
				appendout(gs,out,"){");
				print_etree(gs,out,f->data.user);
				appendout(gs,out,"})");
			} else {
				(*errorout)(_("Unexpected function type!"));
				appendout(gs,out,"){???}");
			}
			break;
		}
	default:
		(*errorout)(_("Unexpected node!"));
		appendout(gs,out,"(???)");
	       break;
	}
}

void
pretty_print_etree(GString *gs, FILE *out, ETree *n)
{
	/*do a nice printout of matrices if that's the
	  top node*/
	if(n->type == MATRIX_NODE) {
		int i,j;
		appendout(gs,out,"\n[");
		for(j=0;j<matrixw_height(n->data.matrix);j++) {
			if(j>0) appendout(gs,out,"\n ");
			for(i=0;i<matrixw_width(n->data.matrix);i++) {
				if(i>0) appendout(gs,out,"\t");
				print_etree(gs, out,
					    matrixw_index(n->data.matrix,i,j));
			}
		}
		appendout(gs,out,"]");
	} else
		print_etree(gs,out,n);
}

/*add the right parenthesis and brackets to the end of the expression*/
char *
addparenth(char *s)
{
	GList *stack = NULL;
	char *p;
	int l;

	for(p=s;*p!='\0';p++) {
		switch(*p) {
			case '[':
				stack_push(&stack,"]");
				break;
			case '(':
				stack_push(&stack,")");
				break;
			case '{':
				stack_push(&stack,"}");
				break;
			case ']':
				if(!stack)
					return s; /*too many trailing*/
				if(*(char *)stack_pop(&stack) != ']') {
					if(stack) g_list_free(stack);
					return s; /*non matching*/
				}
				break;
			case ')':
				if(!stack)
					return s; /*too many trailing*/
				if(*(char *)stack_pop(&stack) != ')') {
					if(stack) g_list_free(stack);
					return s; /*non matching*/
				}
				break;
			case '}':
				if(!stack)
					return s; /*too many trailing*/
				if(*(char *)stack_pop(&stack) != '}') {
					if(stack) g_list_free(stack);
					return s; /*non matching*/
				}
				break;
		}
	}
	l = strlen(s);
	s = g_realloc(s,l+g_list_length(stack)+1);
	while(stack)
		s[l++]=*((char *)stack_pop(&stack));
	s[l]='\0';
	return s;
}


static void
load_file(char *file, calcstate_t state, void (*errorfunc)(char *))
{
	FILE *fp;
	int oldgeof = got_eof;
	got_eof = FALSE;
	if((fp = fopen(file,"r"))) {
		while(1) {
			g_free(evalexp(NULL,fp,NULL,NULL,state,errorfunc,FALSE));
			if(got_eof) {
				got_eof = FALSE;
				break;
			}
		}
		fclose(fp);
		got_eof = oldgeof;
	} else {
		char buf[256];
		g_snprintf(buf,256,_("Can't open file: '%s'"),file);
		(*errorfunc)(buf);
		got_eof = oldgeof;
	}
}

static void
interrupt(int x)
{
	interrupted = TRUE;
	signal(SIGINT,SIG_IGN);
}

char *
evalexp(char * str, FILE *infile, FILE *outfile, char *prefix,
	calcstate_t state,void (*errorfunc)(char *),int pretty)
{
	char * p;
	int stacklen;
	ETree *ret;
	int willquit = FALSE;
	
	/*init the context stack and clear out any stale dictionaries
	  except the global one, if this is the first time called it
	  will also register the builtin routines with the global
	  dictionary*/
	d_singlecontext();
	
	errorout=errorfunc;

	error_num=NO_ERROR;

	/*set the state variable for calculator*/
	calcstate=state;

	mpw_init_mp();
	mpw_set_default_prec(state.float_prec);
	
	first_tok = STARTTOK;

	use_readline = FALSE;
	if(str || !infile) {
		pipe(lex_fd);
		yyin=fdopen(lex_fd[0],"r");
		if(str) {
			write(lex_fd[1],str,strlen(str));
		}
#ifdef WITH_READLINE_SUPPORT
	 	else {
			char *s = readline("genius> ");
			if(!s) {
				got_eof = TRUE;
				return NULL;
			}
			if(*s)
				add_history(s);
			write(lex_fd[1],s,strlen(s));
			free(s);
			use_readline = TRUE;
		}
#endif
		write(lex_fd[1],"\n",1);
		close(lex_fd[1]);
	} else
		yyin = infile;

	g_free(loadfile); loadfile = NULL;
	g_free(loadfile_glob); loadfile_glob = NULL;

	lex_init = TRUE;
	/*yydebug=TRUE;*/  /*turn debugging of parsing on here!*/
	yyparse();

	/*while(yyparse() && !feof(yyin))
		;*/

	if(str || !infile) {
		close(lex_fd[0]);
		fclose(yyin);
		yyin = NULL;
	}

	if(loadfile) {
		char *file = loadfile;
		loadfile=NULL;
		while(evalstack)
			freetree(stack_pop(&evalstack));
		load_file(file,state,errorfunc);
		g_free(file);
		return NULL;
	}

	if(loadfile_glob) {
		glob_t gs;
		char *f;
		char *flist = loadfile_glob;
		int i;
		loadfile_glob = NULL;
		while(evalstack)
			freetree(stack_pop(&evalstack));
		f = strtok(flist,"\t ");
		while(f) {
			glob(f,GLOB_NOSORT|GLOB_NOCHECK,NULL,&gs);
			for(i=0;i<gs.gl_pathc;i++) {
				load_file(gs.gl_pathv[i],state,errorfunc);
			}
			globfree(&gs);
			f = strtok(NULL,"\t ");
		}
		free(flist);
		return NULL;
	}
	
	/*catch parsing errors*/
	if(error_num!=NO_ERROR) {
		while(evalstack)
			freetree(stack_pop(&evalstack));
		return NULL;
	}
	
	stacklen = g_list_length(evalstack);
	
	if(stacklen==0)
		return NULL;

	/*stack is supposed to have only ONE entry*/
	if(stacklen!=1) {
		while(evalstack)
			freetree(stack_pop(&evalstack));
		(*errorout)(_("ERROR: Probably corrupt stack!"));
		return NULL;
	}

#ifdef LEAK_DEBUG_CONTRAPTION
for(;;) {
	ETree *node = copynode(evalstack->data);
#endif
	if(returnval)
		freetree(returnval);
	returnval = NULL;
	inbailout = FALSE;
	inexception = FALSE;
	interrupted = FALSE;
	if(inloop) g_slist_free(inloop);
	inloop = g_slist_prepend(NULL,GINT_TO_POINTER(0));
	signal(SIGINT,interrupt);
	ret = evalnode(evalstack->data);
	signal(SIGINT,SIG_IGN);
	g_slist_free(inloop);
	inloop = NULL;
	if(interrupted || inbailout || inexception) {
		if(returnval) freetree(returnval);
		returnval = NULL;
	}
	inbailout = FALSE;
	inexception = FALSE;
	interrupted = FALSE;
	freetree(stack_pop(&evalstack));
	if(!ret && returnval)
		ret = returnval;
	else if(returnval)
		freetree(returnval);
	returnval = NULL;
#ifdef LEAK_DEBUG_CONTRAPTION
	if(infile) break;
	if(ret) freetree(ret);
	ret = NULL;
	stack_push(&evalstack,node);
}
#endif

	/*catch evaluation errors*/
	if(!ret)
		return NULL;
	if(error_num!=NO_ERROR) {
		freetree(ret);
		return NULL;
	}

	p = NULL;
	if(ret->type != NULL_NODE) {
		GString *gs = NULL;
		if(!outfile)
			gs = g_string_new(NULL);
		if(prefix) {
			if(outfile)
				fputs(prefix,outfile);
			else
				g_string_append(gs,prefix);
		}
		if(pretty)
			pretty_print_etree(gs,outfile,ret);
		else
			print_etree(gs,outfile,ret);
		if(outfile) {
			fputc('\n',outfile);
		} else {
			p = gs->str;
			g_string_free(gs,FALSE);
		}
	}

	/*set ans to the last answer*/
	if(ret->type == FUNCTION_NODE) {
		if(ret->data.func)
			d_addfunc(d_makerealfunc(ret->data.func,d_intern("Ans")));
		else
			d_addfunc(d_makeufunc(d_intern("Ans"),makenum_ui(0),NULL,0));
		freetree(ret);
	} else if(ret->type == OPERATOR_NODE &&
		ret->data.oper == E_REFERENCE) {
		ETree *t = ret->args->data;
		if(!t) {
			EFunc *rf = d_lookup_global(t->data.id);
			if(rf)
				d_addfunc(d_makereffunc(d_intern("Ans"),rf));
			else
				d_addfunc(d_makeufunc(d_intern("Ans"),makenum_ui(0),NULL,0));
		} else
				d_addfunc(d_makeufunc(d_intern("Ans"),makenum_ui(0),NULL,0));
		freetree(ret);
	} else
		d_addfunc(d_makeufunc(d_intern("Ans"),ret,NULL,0));

	return p;
}

/*just to make the compiler happy*/
void yyerror(char *s);

void
yyerror(char *s)
{
	char *out=NULL;
	out=g_strconcat(_("ERROR: "),s,_(" before '"),yytext,"'",NULL);
	
	(*errorout)(out);
	g_free(out);
	error_num=PARSE_ERROR;
}

