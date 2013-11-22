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

#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdlib.h>
#include <gtk/gtk.h>

/*simple realloc/free functions using g_malloc/g_free confogming to gmp*/
void * my_malloc(size_t size);
void * my_realloc(void *ptr,size_t olds,size_t news);
void my_free(void *ptr,size_t s);

/*shift the sring to the right by n*/
void shiftstr(char *s,int n);

/*allocate new space in s for p and append it*/
char * appendstr(char *s,char *p);

/*allocate new space in s for p and prepend it*/
char * prependstr(char *s,char *p);

/*stack operations using GList*/
void stack_push(GList **stack, gpointer data);
gpointer stack_pop(GList **stack);

#endif