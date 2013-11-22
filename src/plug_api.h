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

#ifndef _PLUG_API_H_
#define _PLUG_API_H_

typedef struct _PluginInfo PluginInfo;
struct _PluginInfo {
	void (*open)(void); /*open the plugin (this happens every
			      time the user selects the menuitem)*/
	int (*save_state)(char *prefix); /*save state using gnome_config
					   in this file, and return TRUE
					   if genius should reload this
					   plugin next time*/
	void (*restore_state)(char *prefix);
};

/*this is here to avoid "prototype warnings", this is a function which the
  plugins should define*/
PluginInfo * init_func(void);

#endif
