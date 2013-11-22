/* GnomENIUS Calculator
 * Copyright (C) 1999 the Free Software Foundation.
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
#include <gmodule.h>
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

#include "plug_api.h"
#include "plugin.h"
#include "plugread.h"

GList *plugin_list = NULL;

static GHashTable *opened = NULL;
static GHashTable *info = NULL;

extern void (*errorout)(char *);
extern void (*infoout)(char *);

void
read_plugin_list(void)
{
	GList *li;
	DIR *dir;
	char *dir_name;
	struct dirent *dent;

	/*free the previous list*/
	for(li=plugin_list;li;li=g_list_next(li)) {
		plugin_t *plg = li->data;
		g_free(plg->file);
		g_free(plg->name);
		g_free(plg);
	}
	g_list_free(plugin_list);
	plugin_list = NULL;
	
	dir_name = g_strconcat(LIBRARY_DIR,"/plugins",NULL);
	dir = opendir(dir_name);
	if(!dir) {
		g_free(dir_name);
		return;
	}
	while((dent = readdir (dir)) != NULL) {
		char *p;
		plugin_t *plg;
		if(dent->d_name[0] == '.' &&
		   (dent->d_name[1] == '\0' ||
		    (dent->d_name[1] == '.' &&
		     dent->d_name[2] == '\0')))
			continue;
		p = strrchr(dent->d_name,'.');
		if(!p || strcmp(p,".plugin")!=0)
			continue;
		plg = readplugin(dir_name,dent->d_name);
		if(plg)
			plugin_list = g_list_prepend(plugin_list,plg);
	}
	g_free(dir_name);
	plugin_list = g_list_reverse(plugin_list);
}

void
open_plugin(plugin_t *plug)
{
	GModule *mod;
	PluginInfo *inf;
	if(!opened)
		opened = g_hash_table_new(g_str_hash,g_str_equal);
	if(!info)
		info = g_hash_table_new(NULL,NULL);
	
	if(!(mod=g_hash_table_lookup(opened,plug->file))) {
		mod = g_module_open(plug->file,G_MODULE_BIND_LAZY);
		if(!mod) {
			(*errorout)(_("Can't open plugin!"));
		 	return;
		}
		g_module_make_resident(mod);
		g_hash_table_insert(opened,g_strdup(plug->file),mod);
	}
	if(!(inf=g_hash_table_lookup(info,mod))) {
		PluginInfo *(*init_func)(void);
		
		if(!g_module_symbol(mod,"init_func",(gpointer *)&init_func) ||
		   !init_func || 
		   !(inf=(*init_func)())) {
			(*errorout)(_("Can't initialize plugin!"));
			return;
		}
		g_hash_table_insert(info,mod,inf);
	}
	
	(*inf->open)();
}
