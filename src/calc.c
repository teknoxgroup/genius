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

#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <signal.h>
#include <glob.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <glib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <readline/tilde.h>

#include "calc.h"
#include "eval.h"
#include "util.h"
#include "dict.h"
#include "funclib.h"
#include "matrixw.h"
#include "compil.h"

#include "mpwrap.h"

#include "parse.h"

extern int lex_fd[2];
extern int first_tok;
extern int lex_init;

extern FILE *yyin;
extern char *yytext;
extern int yydebug;

extern ETree *returnval;
extern int inbailout;
extern int inexception;

extern GSList *inloop;
extern char *loadfile;
extern char *loadfile_glob;
void (*evalnode_hook)(void) = NULL;
int run_hook_every = 1000;
void (*statechange_hook)(calcstate_t) = NULL;

static GHashTable *funcdesc = NULL;

static int ignore_end_parse_errors = FALSE;
static int got_end_too_soon = FALSE;

FILE *outputfp = NULL;

GHashTable *uncompiled = NULL;

/* stack ... has to be global:-( */
evalstack_t evalstack=NULL;

/*error .. global as well*/
calc_error_t error_num = NO_ERROR;
int got_eof = FALSE;

/*the current state of the calculator*/
calcstate_t calcstate = {0};

/*error reporting function*/
void (*errorout)(char *)=NULL;
void (*infoout)(char *)=NULL;

char *loadfile = NULL;
char *loadfile_glob = NULL;

int interrupted = FALSE;

static GList *curfile = NULL;
static GList *curline = NULL;

/*from lexer.l*/
int my_yyinput(void);

void
add_description(char *func, char *desc)
{
	char *origkey;
	char *origdesc;
	char *p;
	
	/*kill \n's \r's and ;'s (for compiled parsing purposes)
	  and cap the entry at 80*/
	desc = g_strdup(desc);
	if((p=strchr(desc,'\n')))
		*p = '\0';
	if((p=strchr(desc,'\r')))
		*p = '\0';
	if(strlen(desc)>80) 
		desc[81]='\0';

	if(!funcdesc)
		funcdesc = g_hash_table_new(g_str_hash,g_str_equal);
	
	if(g_hash_table_lookup_extended(funcdesc,func,
					(gpointer *)&origkey,
					(gpointer *)&origdesc)) {
		g_free(origdesc);
		g_hash_table_insert(funcdesc,origkey,g_strdup(desc));
	} else
		g_hash_table_insert(funcdesc,g_strdup(func),g_strdup(desc));
	g_free(desc);
}

char *
get_description(char *func)
{
	char *s;
	if(!funcdesc)
		return "";
	
	s = g_hash_table_lookup(funcdesc,func);
	if(s) return s;
	else return "";
}

void
push_file_info(char *file,int line)
{
	curfile = g_list_prepend(curfile,file?g_strdup(file):NULL);
	curline = g_list_prepend(curline,GINT_TO_POINTER(line));
}

void
pop_file_info(void)
{
	GList *li;
	g_assert(curfile && curline);

	li = curfile;
	curfile = g_list_remove_link(curfile,curfile);
	g_free(li->data);
	g_list_free_1(li);
	li = curline;
	curline = g_list_remove_link(curline,curline);
	g_list_free_1(li);
}

void
incr_file_info(void)
{
	int i;
	
	if(!curline)
		return;
	
	i = GPOINTER_TO_INT(curline->data);
	curline->data = GINT_TO_POINTER((i+1));
}

void
rewind_file_info(void)
{
	int i;
	
	if(!curline)
		return;
	
	curline->data = GINT_TO_POINTER(1);
}

void
get_file_info(char **file, int *line)
{
	int i;
	
	if(!curline || !curfile) {
		*file = NULL;
		*line = 0;
		return;
	}
	
	*file = curfile->data;
	*line = GPOINTER_TO_INT(curline->data);
}

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

	switch(n->op.oper) {
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

		case E_GET_VELEMENT:
			GET_LR(n,l,r);
			appendout_c(gs,out,'(');
			print_etree(gs,out,l);
			appendout(gs,out,"@(");
			print_etree(gs,out,r);
			appendout(gs,out,"))");
			break;

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
		case E_FOR_CONS:
			{
				ETree *a,*b,*c,*d;
				GET_ABCD(n,a,b,c,d);
				appendout(gs,out,"(for ");
				print_etree(gs,out,a);
				appendout(gs,out," = ");
				print_etree(gs,out,b);
				appendout(gs,out," to ");
				print_etree(gs,out,c);
				appendout(gs,out," do ");
				print_etree(gs,out,d);
				appendout(gs,out,")");
				break;
			}
		case E_FORBY_CONS:
			{
				ETree *a,*b,*c,*d,*e;
				GET_ABCDE(n,a,b,c,d,e);
				appendout(gs,out,"(for ");
				print_etree(gs,out,a);
				appendout(gs,out," = ");
				print_etree(gs,out,b);
				appendout(gs,out," to ");
				print_etree(gs,out,c);
				appendout(gs,out," by ");
				print_etree(gs,out,d);
				appendout(gs,out," do ");
				print_etree(gs,out,e);
				appendout(gs,out,")");
				break;
			}
		case E_FORIN_CONS:
			GET_LRR(n,l,r,rr);
			appendout(gs,out,"(for ");
			print_etree(gs,out,l);
			appendout(gs,out," in ");
			print_etree(gs,out,r);
			appendout(gs,out," do ");
			print_etree(gs,out,rr);
			appendout(gs,out,")");
			break;

		case E_DIRECTCALL:
		case E_CALL:
			GET_L(n,l);
			appendout_c(gs,out,'(');
			if(l->type==IDENTIFIER_NODE) {
				appendout(gs,out,l->id.id->token);
			} else if(l->type == OPERATOR_NODE && l->op.oper == E_DEREFERENCE) {
				ETree *t;
				GET_L(l,t);
				if(t->type!=IDENTIFIER_NODE) {
					(*errorout)(_("Bad identifier for function node!"));
					appendout(gs,out,"???)");
					break;
				}
				appendout_c(gs,out,'*');
				appendout(gs,out,t->id.id->token);
			} else {
				(*errorout)(_("Bad identifier for function node!"));
				appendout(gs,out,"???)");
				break;
			}
			appendout_c(gs,out,'(');
			li = n->op.args->next;
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
appendcomp(GString *gs,FILE *out, ETree *n)
{
	GList *li,*oli;
	
	appendout_c(gs,out,'(');
	
	for(oli=n->comp.comp,li=n->comp.args;oli;
	    li=g_list_next(li),oli=g_list_next(oli)) {
		int oper= GPOINTER_TO_INT(oli->data);
		print_etree(gs,out,li->data);
		switch(oper) {
		case E_EQ_CMP:
			appendout(gs,out,"=="); break;
		case E_NE_CMP:
			appendout(gs,out,"!="); break;
		case E_LT_CMP:
			appendout(gs,out,"<"); break;
		case E_GT_CMP:
			appendout(gs,out,">"); break;
		case E_LE_CMP:
			appendout(gs,out,"<="); break;
		case E_GE_CMP:
			appendout(gs,out,">="); break;
		default:
			g_assert_not_reached();
		}
	}
	print_etree(gs,out,li->data);

	appendout_c(gs,out,')');
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
		p=mpw_getstring(n->val.value,calcstate.max_digits,
				calcstate.scientific_notation,
				calcstate.results_as_floats);
		appendout(gs,out,p);
		g_free(p);
		break;
	case MATRIX_NODE:
		if(n->mat.quoted)
			appendout_c(gs,out,'`');
		appendmatrix(gs,out,n->mat.matrix);
		break;
	case OPERATOR_NODE:
		appendoper(gs,out,n);
		break;
	case IDENTIFIER_NODE:
		appendout(gs,out,n->id.id->token);
		break;
	case STRING_NODE:
		appendout_c(gs,out,'"');
		p = escape_string(n->str.str);
		appendout(gs,out,p);
		g_free(p);
		appendout_c(gs,out,'"');
		break;
	case FUNCTION_NODE:
		{
			GList *li;
			EFunc *f;
			
			f = n->func.func;
			if(!f) {
				(*errorout)(_("NULL function!"));
				appendout(gs,out,"(???)");
				break;
			}
			if(f->type==BUILTIN_FUNC) {
				appendout(gs,out,"(<builtin function>)");
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
				appendout(gs,out,")=(");
				if(!f->data.user) {
					g_assert(uncompiled);
					f->data.user =
						decompile_tree(g_hash_table_lookup(uncompiled,f->id));
					g_hash_table_remove(uncompiled,f->id);
					g_assert(f->data.user);
				}
				print_etree(gs,out,f->data.user);
				appendout(gs,out,"))");
			} else {
				/*variable and reference functions should
				  never be in the etree*/
				(*errorout)(_("Unexpected function type!"));
				appendout(gs,out,")(???)");
			}
			break;
		}
	case COMPARISON_NODE:
		appendcomp(gs,out,n);
		break;
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
		if(n->mat.quoted)
			appendout(gs,out,"\n`[");
		else
			appendout(gs,out,"\n[");
		for(j=0;j<matrixw_height(n->mat.matrix);j++) {
			if(j>0) {
				if(n->mat.quoted)
					appendout(gs,out,"\n  ");
				else
					appendout(gs,out,"\n ");
			}
			for(i=0;i<matrixw_width(n->mat.matrix);i++) {
				if(i>0) appendout(gs,out,"\t");
				print_etree(gs, out,
					    matrixw_index(n->mat.matrix,i,j));
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

void
compile_all_user_funcs(FILE *outfile)
{
	GList *funcs;
	fprintf(outfile,"CGEL "VERSION"\n");
	funcs = d_getcontext();
	for(funcs=g_list_last(funcs);funcs;funcs=g_list_previous(funcs)) {
		EFunc *func = funcs->data;
		char *body;
		GList *li;
		char *d;

		if((func->type!=USER_FUNC &&
		    func->type!=VARIABLE_FUNC) ||
		   !func->id ||
		   !func->id->token ||
		   strcmp(func->id->token,"Ans")==0)
			continue;

		if(func->data.user) {
			body = compile_tree(func->data.user);
		} else {
			body = g_strdup(g_hash_table_lookup(uncompiled,func->id));
			g_assert(body);
		}
		if(func->type==USER_FUNC) {
			fprintf(outfile,"F;%d;%s;%d",(int)strlen(body),func->id->token,(int)func->nargs);
			for(li=func->named_args;li;li=g_list_next(li)) {
				Token *tok = li->data;
				fprintf(outfile,";%s",tok->token);
			}
		} else /*VARIABLE_FUNC*/ {
			fprintf(outfile,"V;%d;%s",(int)strlen(body),func->id->token);
		}

		fprintf(outfile,"\n%s\n",body);
		g_free(body);
		d = get_description(func->id->token);
		if(*d)
			fprintf(outfile,"D;%s;%s\n",func->id->token,d);
	}
}

static void
load_compiled_fp(char *file, FILE *fp)
{
	char buf[4096];

	if(!fgets(buf,4096,fp))
		return;
	if(strcmp(buf,"CGEL "VERSION"\n")!=0) {
		g_snprintf(buf,4096,_("File '%s' is a wrong version of GEL"),file);
		(*errorout)(buf);
		return;
	}

	/*init the context stack and clear out any stale dictionaries
	  except the global one, if this is the first time called it
	  will also register the builtin routines with the global
	  dictionary*/
	d_singlecontext();

	error_num=NO_ERROR;

	/*if we this was set, then the mp library was initialized for
	  sure*/
	g_assert(calcstate.float_prec>0);

	while(fgets(buf,4096,fp)) {
		char *p;
		char *b2;
		Token *tok;
		int size,nargs;
		int i;
		GList *li = NULL;
		int type;

		incr_file_info();

		p=strchr(buf,'\n');
		if(p) *p='\0';

		p = strtok(buf,";");
		if(!p) {
			(*errorout)(_("Badly formed record"));
			continue;
		} else if(*p == 'T') {
			(*errorout)(_("Record out of place"));
			continue;
		} else if(*p == 'D') {
			char *d;
			p = strtok(NULL,";");
			if(!p) {
				(*errorout)(_("Badly formed record"));
				continue;
			}
			d = strtok(NULL,";");
			if(!d) {
				(*errorout)(_("Badly formed record"));
				continue;
			}
			add_description(p,d);
			continue;
		} else if(*p != 'F' && *p != 'V') {
			(*errorout)(_("Badly formed record"));
			continue;
		}
		type = *p=='F'?USER_FUNC:VARIABLE_FUNC;

		/*size*/
		p = strtok(NULL,";");
		if(!p) {
			(*errorout)(_("Badly formed record"));
			continue;
		}
		size = -1;
		sscanf(p,"%d",&size);
		if(size==-1) {
			(*errorout)(_("Badly formed record"));
			continue;
		}

		/*id*/
		p = strtok(NULL,";");
		if(!p) {
			(*errorout)(_("Badly formed record"));
			continue;
		}
		tok = d_intern(p);

		if(type == USER_FUNC) {
			/*nargs*/
			p = strtok(NULL,";");
			if(!p) {
				(*errorout)(_("Badly formed record"));
				continue;
			}
			nargs = -1;
			sscanf(p,"%d",&nargs);
			if(size==-1) {
				(*errorout)(_("Badly formed record"));
				continue;
			}

			/*argument names*/
			li = NULL;
			for(i=0;i<nargs;i++) {
				p = strtok(NULL,";");
				if(!p) {
					(*errorout)(_("Badly formed record"));
					g_list_free(li);
					goto continue_reading;
				}
				li = g_list_append(li,d_intern(p));
			}
		}

		/*the value*/
		b2 = g_new(char,size+2);
		if(!fgets(b2,size+2,fp)) {
			(*errorout)(_("Missing value for function"));
			g_free(b2);
			g_list_free(li);
			goto continue_reading;
		}
		incr_file_info();
		p=strchr(b2,'\n');
		if(p) *p='\0';
		if(!uncompiled)
			uncompiled = g_hash_table_new(NULL,NULL);
		g_hash_table_insert(uncompiled,tok,b2);
		if(type == USER_FUNC)
			d_addfunc(d_makeufunc(tok,NULL,li,nargs));
		else /*VARIABLE_FUNC*/
			d_addfunc(d_makevfunc(tok,NULL));
continue_reading:	;
	}
	fclose(fp);
}

void
load_compiled_file(char *file, int warn)
{
	FILE *fp;
	if((fp = fopen(file,"r"))) {
		push_file_info(file,1);
		load_compiled_fp(file,fp);
		pop_file_info();
	} else if(warn) {
		char buf[256];
		g_snprintf(buf,256,_("Can't open file: '%s'"),file);
		(*errorout)(buf);
	}
}

void
set_new_calcstate(calcstate_t state)
{
	if(calcstate.float_prec != state.float_prec) {
		mpw_init_mp(); /*just in case we haven't yet*/
		mpw_set_default_prec(state.float_prec);
		break_fp_caches();
	}
	/*set the state variable for calculator*/
	calcstate=state;

	/*if the calling process already knows, who cares,
	  but call it anyway*/
	if(statechange_hook)
		(*statechange_hook)(state);
}

static void
load_fp(FILE *fp)
{
	while(1) {
		evalexp(NULL,fp,NULL,NULL,NULL,FALSE);
		if(got_eof) {
			got_eof = FALSE;
			break;
		}
		if(interrupted)
			break;
	}
	fclose(fp);
}

void
load_file(char *file, int warn)
{
	FILE *fp;
	int oldgeof = got_eof;
	got_eof = FALSE;
	if((fp = fopen(file,"r"))) {
		push_file_info(file,1);
		load_fp(fp);
		pop_file_info();
		got_eof = oldgeof;
	} else if(warn) {
		char buf[256];
		g_snprintf(buf,256,_("Can't open file: '%s'"),file);
		(*errorout)(buf);
		got_eof = oldgeof;
	}
}

void
load_guess_file(char *file, int warn)
{
	FILE *fp;
	int oldgeof = got_eof;
	got_eof = FALSE;
	if((fp = fopen(file,"r"))) {
		char buf[6];
		push_file_info(file,1);
		if(fgets(buf,6,fp) &&
		   strncmp(buf,"CGEL ",5)==0) {
			rewind(fp);
			load_compiled_fp(file,fp);
		} else {
			rewind(fp);
			load_fp(fp);
		}
		pop_file_info();
		got_eof = oldgeof;
	} else if(warn) {
		char buf[256];
		g_snprintf(buf,256,_("Can't open file: '%s'"),file);
		(*errorout)(buf);
		got_eof = oldgeof;
	}
}

void
set_new_errorout(void (*func)(char *))
{
	errorout = func;
}

void
set_new_infoout(void (*func)(char *))
{
	infoout = func;
}

static void
do_load_files(void)
{
	if(loadfile) {
		char *file = loadfile;
		loadfile=NULL;
		while(evalstack)
			freetree(stack_pop(&evalstack));
		load_file(file,TRUE);
		g_free(file);
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
				char *s = tilde_expand_word(gs.gl_pathv[i]);
				load_guess_file(s,TRUE);
				free(s);
				if(interrupted) {
					globfree(&gs);
					free(flist);
					return;
				}
			}
			globfree(&gs);
			f = strtok(NULL,"\t ");
		}
		free(flist);
	}
}

ETree *
parseexp(char *str, FILE *infile, int load_files, int testparse, int *finished)
{
	int stacklen;
	
	interrupted = FALSE;
	
	/*init the context stack and clear out any stale dictionaries
	  except the global one, if this is the first time called it
	  will also register the builtin routines with the global
	  dictionary*/
	d_singlecontext();
	
	error_num=NO_ERROR;
	
	/*if we this was set, then the mp library was initialized for
	  sure*/
	g_assert(calcstate.float_prec>0);

	first_tok = STARTTOK;
	
	g_assert(str || infile);
	g_assert(!(str && infile));

	if(str) {
		int l = strlen(str);
		pipe(lex_fd);
		yyin=fdopen(lex_fd[0],"r");
		write(lex_fd[1],str,l);
		if(str[l-1]!='\n')
			write(lex_fd[1],"\n",1);
		close(lex_fd[1]);
	} else
		yyin = infile;

	g_free(loadfile); loadfile = NULL;
	g_free(loadfile_glob); loadfile_glob = NULL;

	lex_init = TRUE;
	/*yydebug=TRUE;*/  /*turn debugging of parsing on here!*/
	if(testparse) ignore_end_parse_errors = TRUE;
	got_end_too_soon = FALSE;
	yyparse();
	ignore_end_parse_errors = FALSE;

	/*while(yyparse() && !feof(yyin))
		;*/

	if(str) {
		while(my_yyinput()!=EOF)
			;
		close(lex_fd[0]);
		fflush(yyin);
		fclose(yyin);
		yyin = NULL;
	}
	
	if(!load_files) {
		g_free(loadfile); loadfile = NULL;
		g_free(loadfile_glob); loadfile_glob = NULL;
	} else if(loadfile || loadfile_glob) {
		do_load_files();
		if(finished) *finished = TRUE;
		return NULL;
	}

	/*if we are testing and got an unfinished expression just report that*/
	if(testparse && got_end_too_soon) {
		while(evalstack)
			freetree(stack_pop(&evalstack));
		if(finished) *finished = FALSE;
		return NULL;
	}
	
	/*catch parsing errors*/
	if(error_num!=NO_ERROR) {
		while(evalstack)
			freetree(stack_pop(&evalstack));
		if(finished) *finished = TRUE;
		return NULL;
	}
	
	stacklen = g_list_length(evalstack);
	
	if(stacklen==0) {
		if(finished) *finished = FALSE;
		return NULL;
	}

	/*stack is supposed to have only ONE entry*/
	if(stacklen!=1) {
		while(evalstack)
			freetree(stack_pop(&evalstack));
		if(!testparse)
			(*errorout)(_("ERROR: Probably corrupt stack!"));
		if(finished) *finished = FALSE;
		return NULL;
	}
	evalstack->data = gather_comparisons(evalstack->data);
	
	if(finished) *finished = TRUE;
	return stack_pop(&evalstack);
}

ETree *
runexp(ETree *exp)
{
	static int busy = FALSE;
	ETree *ret;
	
	if(busy) {
		(*errorout)(_("ERROR: Can't execute more things at once!"));
		return NULL;
	}
	
	busy = TRUE;

	error_num=NO_ERROR;
	
	push_file_info(NULL,0);
	if(returnval)
		freetree(returnval);
	returnval = NULL;
	inbailout = FALSE;
	inexception = FALSE;
	interrupted = FALSE;
	if(inloop) g_slist_free(inloop);
	inloop = g_slist_prepend(NULL,GINT_TO_POINTER(0));

	ret = evalnode(exp);

	g_slist_free(inloop);
	inloop = NULL;
	if(interrupted || inbailout || inexception) {
		if(returnval) freetree(returnval);
		returnval = NULL;
	}
	inbailout = FALSE;
	inexception = FALSE;
	/* we need to catch this later as well: interrupted = FALSE;*/
	if(!ret && returnval)
		ret = returnval;
	else if(returnval)
		freetree(returnval);
	returnval = NULL;
	pop_file_info();

	busy = FALSE;

	/*catch evaluation errors*/
	if(!ret)
		return NULL;
	if(error_num!=NO_ERROR) {
		freetree(ret);
		return NULL;
	}
	return ret;
}

void
evalexp_parsed(ETree *parsed, FILE *outfile, char **outstring, char *prefix,int pretty)
{
	ETree *ret;
	
	if(!outputfp)
		outputfp=stdout;
	
	if(outstring)
		*outstring = NULL;
	
	if(!parsed) return;
	ret = runexp(parsed);
	freetree(parsed);
	if(!ret) return;

	if(ret->type != NULL_NODE && (outstring || outfile)) {
		GString *gs = NULL;
		if(outstring)
			gs = g_string_new(NULL);
		if(prefix) {
			if(outfile)
				fputs(prefix,outfile);
			if(outstring)
				g_string_append(gs,prefix);
		}
		if(pretty)
			pretty_print_etree(gs,outfile,ret);
		else
			print_etree(gs,outfile,ret);
		if(outfile)
			fputc('\n',outfile);

		if(outstring) {
			*outstring = gs->str;
			g_string_free(gs,FALSE);
		}
	}

	/*set ans to the last answer*/
	if(ret->type == FUNCTION_NODE) {
		if(ret->func.func)
			d_addfunc(d_makerealfunc(ret->func.func,d_intern("Ans"),TRUE));
		else
			d_addfunc(d_makevfunc(d_intern("Ans"),makenum_ui(0)));
		freetree(ret);
	} else if(ret->type == OPERATOR_NODE &&
		ret->op.oper == E_REFERENCE) {
		ETree *t = ret->op.args->data;
		if(!t) {
			EFunc *rf = d_lookup_global(t->id.id);
			if(rf)
				d_addfunc(d_makereffunc(d_intern("Ans"),rf));
			else
				d_addfunc(d_makevfunc(d_intern("Ans"),makenum_ui(0)));
		} else
				d_addfunc(d_makevfunc(d_intern("Ans"),makenum_ui(0)));
		freetree(ret);
	} else
		d_addfunc(d_makevfunc(d_intern("Ans"),ret));
}

void
evalexp(char * str, FILE *infile, FILE *outfile, char **outstring, char *prefix,int pretty)
{
	ETree *parsed;
	parsed = parseexp(str,infile,TRUE,FALSE,NULL);
	evalexp_parsed(parsed,outfile,outstring,prefix,pretty);
}

/*just to make the compiler happy*/
void yyerror(char *s);

void
yyerror(char *s)
{
	char *out=NULL;
	char *p;
	
	if(ignore_end_parse_errors && yytext[0]=='\0') {
		got_end_too_soon = TRUE;
		return;
	}
	
	if(strcmp(yytext,"\n")==0) {
		out=g_strconcat(_("ERROR: "),s,_(" before newline"),NULL);
	} else if(yytext[0]=='\0') {
		out=g_strconcat(_("ERROR: "),s,_(" at end of input"),NULL);
	} else {
		char *tmp = g_strdup(yytext);
		while((p=strchr(tmp,'\n')))
			*p='.';

		out=g_strconcat(_("ERROR: "),s,_(" before '"),tmp,"'",NULL);
		g_free(tmp);
	}

	(*errorout)(out);
	g_free(out);
	error_num=PARSE_ERROR;
}
