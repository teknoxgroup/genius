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

#include <gnome.h>
#include <gtk/gtk.h>
#include <zvt/zvtterm.h>

#include "config.h"

#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include "calc.h"
#include "util.h"
#include "dict.h"

#include "inter.h"

#include <readline/readline.h>
#include <readline/history.h>

#define d(x) ;

/*Globals:*/

/* default font (from zvtterm.c) */
#define DEFAULT_FONT "-misc-fixed-medium-r-semicondensed--13-120-75-75-c-60-iso8859-1"

/*calculator state*/
calcstate_t curstate={
	256,
	0,
	FALSE,
	FALSE
	};
	
extern calc_error_t error_num;
extern int got_eof;
extern int parenth_depth;
extern int interrupted;

static GtkWidget *setupdialog = NULL;
static GtkWidget *window = NULL;
static GtkWidget *zvt = NULL;
static GString *errors=NULL;
static GString *infos=NULL;
static int chpid = 0;

typedef struct {
	int error_box;
	int info_box;
	int scrollback;
	char *font;
} geniussetup_t;

geniussetup_t cursetup = {
	FALSE,
	TRUE,
	1000,
	NULL
};

static int torl[2];
static int fromrl[2];

/*used inside rl_getc*/
static int need_quitting = FALSE;
static int in_recursive = FALSE;

/*display a message in a messagebox*/
static void
geniuserrorbox(char *s)
{
	GtkWidget *mb;
	
	mb=gnome_message_box_new(s,GNOME_MESSAGE_BOX_ERROR,
				 GNOME_STOCK_BUTTON_OK,NULL);
	gtk_window_set_transient_for(GTK_WINDOW(mb),
				     GTK_WINDOW(window));

	gtk_widget_show(mb);
}

/*get error message*/
static void
geniuserror(char *s)
{
	char *file;
	int line;
	char *str;
	get_file_info(&file,&line);
	if(file)
		str = g_strdup_printf("%s:%d: %s",file,line,s);
	else if(line>0)
		str = g_strdup_printf("line %d: %s",line,s);
	else
		str = g_strdup(s);
	
	if(cursetup.error_box) {
		if(errors) {
			g_string_append_c(errors,'\n');
			g_string_append(errors,str);
		} else {
			errors = g_string_new(str);
		}
	} else {
		fprintf(rl_outstream,"\e[01;31m%s\e[0m\n",str);
	}

	g_free(str);
}

/*display a message in a messagebox*/
static void
geniusinfobox(char *s)
{
	GtkWidget *mb;
	
	mb=gnome_message_box_new(s,GNOME_MESSAGE_BOX_INFO,
				 GNOME_STOCK_BUTTON_OK,NULL);
	gtk_window_set_transient_for(GTK_WINDOW(mb),
				     GTK_WINDOW(window));

	gtk_widget_show(mb);
}

/*get info message*/
static void
geniusinfo(char *s)
{
	char *file;
	int line;
	char *str;
	get_file_info(&file,&line);
	if(file)
		str = g_strdup_printf("%s:%d: %s",file,line,s);
	else if(line>0)
		str = g_strdup_printf("line %d: %s",line,s);
	else
		str = g_strdup(s);
	
	if(cursetup.info_box) {
		if(infos) {
			g_string_append_c(infos,'\n');
			g_string_append(infos,str);
		} else {
			infos = g_string_new(str);
		}
	} else {
		fprintf(rl_outstream,"\e[0;32m%s\e[0m\n",str);
	}

	g_free(str);
}

/*about box*/
static void
aboutcb(GtkWidget * widget, gpointer data)
{
	GtkWidget *mb;
	char *authors[] = {
		"George Lebl (jirka@5z.com)",
		NULL
	};

	mb=gnome_about_new(_("GnomENIUS Calculator"), VERSION,
			     /* copyright notice */
			     "(C) 1997,1998,1999 the Free Software Foundation",
			     (const char **)authors,
			     /* another comments */
			     _("The Gnome calculator style edition of genius"),
			     NULL);
	gtk_window_set_transient_for(GTK_WINDOW(mb),
				     GTK_WINDOW(window));
			   
	gtk_widget_show(mb);
}

static void
set_properties (void)
{
	gnome_config_set_string("/genius/properties/font", cursetup.font?
				cursetup.font:DEFAULT_FONT);
	gnome_config_set_int("/genius/properties/scrollback", cursetup.scrollback);
	gnome_config_set_bool("/genius/properties/error_box", cursetup.error_box);
	gnome_config_set_bool("/genius/properties/info_box", cursetup.info_box);
	gnome_config_set_bool("/genius/properties/max_digits", 
			      (curstate.max_digits == 0) ? TRUE : FALSE);
	gnome_config_set_bool("/genius/properties/results_as_floats",
			     curstate.results_as_floats);
	gnome_config_set_bool("/genius/properties/scientific_notation",
			     curstate.scientific_notation);
	
	gnome_config_sync();
}

/* quit */
static void
quitapp(GtkWidget * widget, gpointer data)
{
	set_properties();
	need_quitting = TRUE;
	gtk_main_quit();
}

/*exact answer callback*/
static void
intspincb(GtkAdjustment *adj, int *data)
{
	*data=adj->value;
	if(setupdialog)
		gnome_property_box_changed(GNOME_PROPERTY_BOX(setupdialog));
}

/*option callback*/
static void
optioncb(GtkWidget * widget, int *data)
{
	if(GTK_TOGGLE_BUTTON(widget)->active)
		*data=TRUE;
	else
		*data=FALSE;
	
	if(setupdialog)
		gnome_property_box_changed(GNOME_PROPERTY_BOX(setupdialog));
}

static void
fontsetcb(GnomeFontPicker *gfp, gchar *font_name, char **font)
{
	g_free(*font);
	*font = g_strdup(font_name);
	if(setupdialog)
		gnome_property_box_changed(GNOME_PROPERTY_BOX(setupdialog));
}


static calcstate_t tmpstate={0};
static geniussetup_t tmpsetup={0};

static void
do_setup(GtkWidget *widget, gint page, gpointer data)
{
	if (page == -1) {     /* Just finished global apply */
		g_free(cursetup.font);
		cursetup = tmpsetup;
		if(tmpsetup.font)
			cursetup.font = g_strdup(tmpsetup.font);
		curstate = tmpstate;

		set_new_calcstate(curstate);
		zvt_term_set_scrollback(ZVT_TERM(zvt),cursetup.scrollback);
		zvt_term_set_font_name(ZVT_TERM(zvt),cursetup.font?
				       cursetup.font:DEFAULT_FONT);
	}
}

static void
destroy_setup(GtkWidget *widget, gpointer data)
{
	setupdialog = NULL;
}

static void
setup_calc(GtkWidget *widget, gpointer data)
{
	GtkWidget *hbox,*frame;
	GtkWidget *box;
	GtkWidget *b, *w;
	GtkAdjustment *adj;

	if (setupdialog) {
		gtk_widget_show_now(GTK_WIDGET(setupdialog));
		gdk_window_raise(GTK_WIDGET(setupdialog)->window);
		return;
	}
	
	tmpstate = curstate;
	g_free(tmpsetup.font);
	tmpsetup = cursetup;
	if(cursetup.font)
		tmpsetup.font = g_strdup(cursetup.font);
	
	setupdialog = gnome_property_box_new();
	gtk_window_set_transient_for(GTK_WINDOW(setupdialog),
				     GTK_WINDOW(window));
	
	gtk_window_set_title(GTK_WINDOW(setupdialog),
			     _("GnomENIUS Calculator setup"));
	
	hbox = gtk_hbox_new(FALSE, GNOME_PAD);
	gtk_container_border_width(GTK_CONTAINER(hbox),GNOME_PAD);
	gnome_property_box_append_page(GNOME_PROPERTY_BOX(setupdialog),
				       hbox,
				       gtk_label_new(_("Output")));

	
	frame=gtk_frame_new(_("Number output options"));
	gtk_box_pack_start(GTK_BOX(hbox),frame,FALSE,FALSE,0);
	box=gtk_vbox_new(FALSE,GNOME_PAD);
	gtk_container_border_width(GTK_CONTAINER(box),GNOME_PAD);
	gtk_container_add(GTK_CONTAINER(frame),box);


	b=gtk_hbox_new(FALSE,GNOME_PAD);
	gtk_box_pack_start(GTK_BOX(box),b,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(b),
		   gtk_label_new(_("Maximum digits to output (0=unlimited)")),
		   FALSE,FALSE,0);
	adj = (GtkAdjustment *)gtk_adjustment_new(tmpstate.max_digits,
						  0,
						  256,
						  1,
						  5,
						  0);
	w = gtk_spin_button_new(adj,1.0,0);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(w),TRUE);
	gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON(w),
					   GTK_UPDATE_ALWAYS);
	gtk_spin_button_set_snap_to_ticks(GTK_SPIN_BUTTON(w),
					  TRUE);
	gtk_widget_set_usize(w,80,0);
	gtk_box_pack_start(GTK_BOX(b),w,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT(adj),"value_changed",
			   GTK_SIGNAL_FUNC(intspincb),&tmpstate.max_digits);


	w=gtk_check_button_new_with_label(_("Results as floats"));
	gtk_box_pack_start(GTK_BOX(box),w,FALSE,FALSE,0);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(w), 
				    tmpstate.results_as_floats);
	gtk_signal_connect(GTK_OBJECT(w), "toggled",
		   GTK_SIGNAL_FUNC(optioncb),
		   (gpointer)&tmpstate.results_as_floats);
	
	w=gtk_check_button_new_with_label(_("Floats in scientific notation"));
	gtk_box_pack_start(GTK_BOX(box),w,FALSE,FALSE,0);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(w), 
				    tmpstate.scientific_notation);
	gtk_signal_connect(GTK_OBJECT(w), "toggled",
		   GTK_SIGNAL_FUNC(optioncb),
		   (gpointer)&tmpstate.scientific_notation);


	frame=gtk_frame_new(_("Error/Info output options"));
	gtk_box_pack_start(GTK_BOX(hbox),frame,FALSE,FALSE,0);
	box=gtk_vbox_new(FALSE,GNOME_PAD);
	gtk_container_add(GTK_CONTAINER(frame),box);

	gtk_container_border_width(GTK_CONTAINER(box),GNOME_PAD);
	

	w=gtk_check_button_new_with_label(_("Display errors in a dialog"));
	gtk_box_pack_start(GTK_BOX(box),w,FALSE,FALSE,0);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(w), 
				    tmpsetup.error_box);
	gtk_signal_connect(GTK_OBJECT(w), "toggled",
		   GTK_SIGNAL_FUNC(optioncb),
		   (gpointer)&tmpsetup.error_box);

	w=gtk_check_button_new_with_label(_("Display information messages in a dialog"));
	gtk_box_pack_start(GTK_BOX(box),w,FALSE,FALSE,0);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(w), 
				    tmpsetup.info_box);
	gtk_signal_connect(GTK_OBJECT(w), "toggled",
		   GTK_SIGNAL_FUNC(optioncb),
		   (gpointer)&tmpsetup.info_box);

	hbox = gtk_hbox_new(FALSE, GNOME_PAD);
	gtk_container_border_width(GTK_CONTAINER(hbox),GNOME_PAD);
	gnome_property_box_append_page(GNOME_PROPERTY_BOX(setupdialog),
				       hbox,
				       gtk_label_new(_("Precision")));

	
	frame=gtk_frame_new(_("Floating point precision"));
	gtk_box_pack_start(GTK_BOX(hbox),frame,FALSE,FALSE,0);
	box=gtk_vbox_new(FALSE,GNOME_PAD);
	gtk_container_border_width(GTK_CONTAINER(box),GNOME_PAD);
	gtk_container_add(GTK_CONTAINER(frame),box);
	
	gtk_box_pack_start(GTK_BOX(box), gtk_label_new(
		_("NOTE: The floating point precision might not take effect\n"
		  "for all numbers immediately, only new numbers calculated\n"
		  "and new variables will be affected.")),
			   FALSE,FALSE,0);


	b=gtk_hbox_new(FALSE,GNOME_PAD);
	gtk_box_pack_start(GTK_BOX(box),b,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(b),
		   gtk_label_new(_("Floating point precision (bits)")),
		   FALSE,FALSE,0);
	adj = (GtkAdjustment *)gtk_adjustment_new(tmpstate.float_prec,
						  60,
						  16384,
						  1,
						  10,
						  0);
	w = gtk_spin_button_new(adj,1.0,0);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(w),TRUE);
	gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON(w),
					   GTK_UPDATE_ALWAYS);
	gtk_spin_button_set_snap_to_ticks(GTK_SPIN_BUTTON(w),
					  TRUE);
	gtk_widget_set_usize(w,80,0);
	gtk_box_pack_start(GTK_BOX(b),w,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT(adj),"value_changed",
			   GTK_SIGNAL_FUNC(intspincb),&tmpstate.float_prec);


	hbox = gtk_hbox_new(FALSE, GNOME_PAD);
	gtk_container_border_width(GTK_CONTAINER(hbox),GNOME_PAD);
	gnome_property_box_append_page(GNOME_PROPERTY_BOX(setupdialog),
				       hbox,
				       gtk_label_new(_("Terminal")));

	
	frame=gtk_frame_new(_("Terminal options"));
	gtk_box_pack_start(GTK_BOX(hbox),frame,FALSE,FALSE,0);
	box=gtk_vbox_new(FALSE,GNOME_PAD);
	gtk_container_border_width(GTK_CONTAINER(box),GNOME_PAD);
	gtk_container_add(GTK_CONTAINER(frame),box);
	
	b=gtk_hbox_new(FALSE,GNOME_PAD);
	gtk_box_pack_start(GTK_BOX(box),b,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(b),
		   gtk_label_new(_("Scrollback lines")),
		   FALSE,FALSE,0);
	adj = (GtkAdjustment *)gtk_adjustment_new(tmpsetup.scrollback,
						  50,
						  10000,
						  1,
						  10,
						  0);
	w = gtk_spin_button_new(adj,1.0,0);
	gtk_spin_button_set_numeric(GTK_SPIN_BUTTON(w),TRUE);
	gtk_spin_button_set_update_policy (GTK_SPIN_BUTTON(w),
					   GTK_UPDATE_ALWAYS);
	gtk_spin_button_set_snap_to_ticks(GTK_SPIN_BUTTON(w),
					  TRUE);
	gtk_widget_set_usize(w,80,0);
	gtk_box_pack_start(GTK_BOX(b),w,FALSE,FALSE,0);
	gtk_signal_connect(GTK_OBJECT(adj),"value_changed",
			   GTK_SIGNAL_FUNC(intspincb),&tmpsetup.scrollback);
	
	
	b=gtk_hbox_new(FALSE,GNOME_PAD);
	gtk_box_pack_start(GTK_BOX(box),b,FALSE,FALSE,0);
	gtk_box_pack_start(GTK_BOX(b),
		   gtk_label_new(_("Font:")),
		   FALSE,FALSE,0);
	
        w = gnome_font_picker_new();
	gnome_font_picker_set_font_name(GNOME_FONT_PICKER(w),
					tmpsetup.font?tmpsetup.font:
					DEFAULT_FONT);
        gnome_font_picker_set_mode(GNOME_FONT_PICKER(w),GNOME_FONT_PICKER_MODE_FONT_INFO);
        gtk_box_pack_start(GTK_BOX(b),w,TRUE,TRUE,0);
        gtk_signal_connect(GTK_OBJECT(w),"font_set",
                           GTK_SIGNAL_FUNC(fontsetcb),
			   &tmpsetup.font);


	gtk_signal_connect(GTK_OBJECT(setupdialog), "apply",
			   GTK_SIGNAL_FUNC(do_setup), NULL);	
	gtk_signal_connect(GTK_OBJECT(setupdialog), "destroy",
			   GTK_SIGNAL_FUNC(destroy_setup), NULL);
	gtk_widget_show_all(setupdialog);
}

static void
interrupt_calc(GtkWidget *widget, gpointer data)
{
	interrupted = TRUE;
}

static void
fs_destroy_cb(GtkWidget *w, GtkWidget **fs)
{
	*fs = NULL;
}

static void
really_load_cb(GtkWidget *w,GtkFileSelection *fs)
{
	char *s;
	s = gtk_file_selection_get_filename(fs);
	if(!s || !g_file_exists(s)) {
		gnome_app_error(GNOME_APP(window), _("Can not open file!"));
		return;
	}
	load_guess_file(s,TRUE);

	if(infos) {
		geniusinfobox(infos->str);
		g_string_free(infos,TRUE);
		infos=NULL;
	}

	if(errors) {
		geniuserrorbox(errors->str);
		g_free(errors);
		errors=NULL;
	}
}

static void
load_cb(GtkWidget *w)
{
	static GtkWidget *fs = NULL;
	
	if(fs) {
		gtk_widget_show_now(fs);
		gdk_window_raise(fs->window);
		return;
	}

	fs = gtk_file_selection_new(_("Load GEL file"));
	
	gtk_window_position (GTK_WINDOW (fs), GTK_WIN_POS_MOUSE);

	gtk_signal_connect (GTK_OBJECT (fs), "destroy",
			    GTK_SIGNAL_FUNC(fs_destroy_cb), &fs);
	
	gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION (fs)->ok_button),
			    "clicked", GTK_SIGNAL_FUNC(really_load_cb),
			    fs);

	gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (fs)->ok_button),
				   "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy),
				   GTK_OBJECT(fs));
	gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION (fs)->cancel_button),
				   "clicked", GTK_SIGNAL_FUNC(gtk_widget_destroy),
				   GTK_OBJECT(fs));

	gtk_widget_show (fs);
}

static GnomeUIInfo file_menu[] = {
	GNOMEUIINFO_ITEM_NONE(N_("_Load"),N_("Load and execute a file in genius"),load_cb),
	GNOMEUIINFO_MENU_EXIT_ITEM(quitapp,NULL),
	GNOMEUIINFO_END,
};

static GnomeUIInfo settings_menu[] = {  
	GNOMEUIINFO_MENU_PREFERENCES_ITEM(setup_calc,NULL),
	GNOMEUIINFO_END,
};

static GnomeUIInfo calc_menu[] = {  
	GNOMEUIINFO_ITEM_STOCK(N_("_Interrupt"),N_("Interrupt current calculation"),interrupt_calc,GNOME_STOCK_MENU_STOP),
	GNOMEUIINFO_END,
};

static GnomeUIInfo help_menu[] = {  
	GNOMEUIINFO_HELP("genius"),
	GNOMEUIINFO_MENU_ABOUT_ITEM(aboutcb,NULL),
	GNOMEUIINFO_END,
};
  
static GnomeUIInfo genius_menu[] = {
	GNOMEUIINFO_MENU_FILE_TREE(file_menu),
	GNOMEUIINFO_SUBTREE(N_("_Calculator"),calc_menu),
	GNOMEUIINFO_MENU_SETTINGS_TREE(settings_menu),
	GNOMEUIINFO_MENU_HELP_TREE(help_menu),
	GNOMEUIINFO_END,
};

/* toolbar */
static GnomeUIInfo toolbar[] = {
	GNOMEUIINFO_ITEM_STOCK(N_("Interrupt"),N_("Interrupt current calculation"),interrupt_calc,GNOME_STOCK_PIXMAP_STOP),
	GNOMEUIINFO_END,
};


#define ELEMENTS(x) (sizeof (x) / sizeof (x [0]))

/*main window creation, slightly copied from same-gnome:)*/
static GtkWidget *
create_main_window(void)
{
	GtkWidget *w;
        w=gnome_app_new("gnome-genius", _("GnomENIUS Calculator"));
	gtk_window_set_wmclass (GTK_WINDOW (w), "gnome-genius", "gnome-genius");
	gtk_window_set_policy (GTK_WINDOW (w), TRUE, FALSE, TRUE);

        gtk_signal_connect(GTK_OBJECT(w), "delete_event",
		GTK_SIGNAL_FUNC(quitapp), NULL);
        gtk_window_set_policy(GTK_WINDOW(w),1,1,0);
        return w;
}

/* gnome_config employment */

static void
get_properties (void)
{
	gchar buf[256];

	g_snprintf(buf,256,"/genius/properties/font=%s",
		   cursetup.font?cursetup.font:DEFAULT_FONT);
	cursetup.font = gnome_config_get_string(buf);
	g_snprintf(buf,256,"/genius/properties/scrollback=%d",
		   cursetup.scrollback);
	cursetup.scrollback = gnome_config_get_int(buf);
	g_snprintf(buf,256,"/genius/properties/error_box=%s",
		   (cursetup.error_box)?"true":"false");
	cursetup.error_box = gnome_config_get_bool(buf);
	g_snprintf(buf,256,"/genius/properties/info_box=%s",
		   (cursetup.info_box)?"true":"false");
	cursetup.info_box = gnome_config_get_bool(buf);
	
	g_snprintf(buf,256,"/genius/properties/max_digits=%s",
		   (curstate.max_digits == 0)?"true":"false");
	curstate.max_digits = (gnome_config_get_bool(buf))? 0 : 12;
	g_snprintf(buf,256,"/genius/properties/results_as_floats=%s",
		   curstate.results_as_floats?"true":"false");
	curstate.results_as_floats = gnome_config_get_bool(buf);
	g_snprintf(buf,256,"/genius/properties/scientific_notation=%s",
		   curstate.scientific_notation?"true":"false");
	curstate.scientific_notation = gnome_config_get_bool(buf);
}

/*stolen from xchat*/
static void
term_change_pos(GtkWidget *widget)
{
	if(widget && widget->window)
		gtk_widget_queue_draw(widget);
}

static GArray *readbuf=NULL;
static int readbufl = 0;

static int
the_getc(FILE *fp)
{
	for(;;) {
		if(readbufl==0) {
			in_recursive = TRUE;
			gtk_main();
			in_recursive = FALSE;
			if(need_quitting) {
				gtk_exit(0);
			}
			if(interrupted) {
				readbuf = g_array_set_size(readbuf,0);
				return '\n';
			}
		}
		if(readbufl>0) {
			int r = g_array_index(readbuf,char,0);
			readbuf = g_array_remove_index(readbuf,0);
			readbufl--;
			return r;
		}
	}
}

static void
feed_to_zvt(gpointer data, gint source, GdkInputCondition condition)
{
	int size;
	char buf[256];
	while((size = read(source,buf,256))>0) {
		/*do our own crlf translation*/
		char *s;
		int i,sz;
		for(i=0,sz=0;i<size;i++,sz++)
			if(buf[i]=='\n') sz++;
		s = g_new(char,sz);
		for(i=0,sz=0;i<size;i++,sz++) {
			if(buf[i]=='\n') {
				s[sz++] = buf[i];
				s[sz] = '\r';
			} else s[sz] = buf[i];
		}
		zvt_term_feed(ZVT_TERM(zvt),s,sz);
		g_free(s);
	}
}

static void
get_new_buffer(gpointer data, gint source, GdkInputCondition condition)
{
	int size;
	char buf[256];
	while((size = read(source,buf,256))>0) {
		if(!readbuf)
			readbuf = g_array_new(FALSE,FALSE,sizeof(char));
		readbuf = g_array_append_vals(readbuf,buf,size);
		readbufl+=size;
	}
	if(readbufl>0 && in_recursive)
		gtk_main_quit();
}

static void
set_state(calcstate_t state)
{
	curstate = state;
}

static void
check_events(void)
{
	if(gtk_events_pending())
		gtk_main_iteration();
}

static int
catch_interrupts(GtkWidget *w, GdkEvent *e)
{
	if(e->type == GDK_KEY_PRESS) {
		if(e->key.keyval == GDK_c &&
		   e->key.state&GDK_CONTROL_MASK) {
			interrupted = TRUE;
			if(in_recursive)
				gtk_main_quit();
			return TRUE;
		}
	}
	return FALSE;
}

int
main(int argc, char *argv[])
{
	GtkWidget *hbox;
	GtkWidget *w;
	GtkTooltips *tips;
	char *file;
	char buf[256];
	
	pipe(torl);
	pipe(fromrl);

	signal(SIGCHLD,SIG_IGN);
	
	evalnode_hook = check_events;
	statechange_hook = set_state;

	bindtextdomain(PACKAGE,GNOMELOCALEDIR);
	textdomain(PACKAGE);

	gnome_init("genius", NULL, argc, argv);

	/*read gnome_config parameters */
	get_properties();
	
        /*set up the top level window*/
	window=create_main_window();

	/*set up the tooltips*/
	tips=gtk_tooltips_new();

	/*the main box to put everything in*/
	hbox=gtk_hbox_new(FALSE,0);
	
	fcntl(torl[0],F_SETFL,O_NONBLOCK);
	gtk_input_add_full(torl[0],GDK_INPUT_READ,get_new_buffer,NULL,NULL,NULL);
	fcntl(fromrl[0],F_SETFL,O_NONBLOCK);
	gtk_input_add_full(fromrl[0],GDK_INPUT_READ,feed_to_zvt,NULL,NULL,NULL);
	
	zvt = zvt_term_new_with_size(80,20);

	zvt_term_set_scrollback(ZVT_TERM(zvt),cursetup.scrollback);
	zvt_term_set_font_name(ZVT_TERM(zvt),cursetup.font?
			       cursetup.font:DEFAULT_FONT);

	zvt_term_set_shadow_type (ZVT_TERM (zvt), GTK_SHADOW_IN);
	zvt_term_set_blink (ZVT_TERM (zvt), TRUE);
	zvt_term_set_bell (ZVT_TERM (zvt), TRUE);
	zvt_term_set_scroll_on_keystroke (ZVT_TERM (zvt), TRUE);
	zvt_term_set_scroll_on_output (ZVT_TERM (zvt), FALSE);
	zvt_term_set_background (ZVT_TERM (zvt), NULL, 0, 0);
	zvt_term_set_wordclass (ZVT_TERM (zvt), "-A-Za-z0-9/_:.,?+%=");
	
	ZVT_TERM(zvt)->vx->vt.keyfd = torl[1];
	gtk_signal_connect(GTK_OBJECT(zvt),"event",
			   GTK_SIGNAL_FUNC(catch_interrupts),
			   NULL);

	gtk_signal_connect_object(GTK_OBJECT(window),"configure_event",
				  GTK_SIGNAL_FUNC(term_change_pos),
				  GTK_OBJECT(zvt));
	
	gtk_box_pack_start(GTK_BOX(hbox),zvt,TRUE,TRUE,0);
	
	w = gtk_vscrollbar_new (GTK_ADJUSTMENT (ZVT_TERM (zvt)->adjustment));
	gtk_box_pack_start(GTK_BOX(hbox),w,FALSE,FALSE,0);

	/*set up the menu*/
        gnome_app_create_menus(GNOME_APP(window), genius_menu);
	/*set up the toolbar*/
	gnome_app_create_toolbar (GNOME_APP(window), toolbar);

	/*setup appbar*/
	w = gnome_appbar_new(FALSE, TRUE, GNOME_PREFERENCES_USER);
	gnome_app_set_statusbar(GNOME_APP(window), w);
	gtk_widget_show(w);

	gnome_app_install_menu_hints(GNOME_APP(window),
				     genius_menu);


	/*set up the main window*/
	gnome_app_set_contents(GNOME_APP(window), hbox);
	gtk_container_border_width(
		GTK_CONTAINER(GNOME_APP(window)->contents),5);

	gtk_widget_show_all(window);
	
	
	init_inter();
	rl_instream = fdopen(torl[0],"r");
	rl_outstream = fdopen(fromrl[1],"w");
	rl_getc_function = the_getc;
	
	outputfp = rl_outstream;

	fprintf(outputfp,
		_("Genius %s\n"
		  "Copyright (c) 1997,1998,1999 Free Software Foundation, Inc.\n"
		  "This is free software with ABSOLUTELY NO WARRANTY.\n"
		  "For details type `warranty'.\n\n"),VERSION);

	set_new_calcstate(curstate);
	set_new_errorout(geniuserror);
	set_new_infoout(geniusinfo);

	file = g_strconcat(LIBRARY_DIR,"/lib.cgel",NULL);
	load_compiled_file(file,FALSE);
	g_free(file);

	/*try the library file in the current directory*/
	load_compiled_file("lib.cgel",FALSE);
	
	file = g_strconcat(getenv("HOME"),"/.geniusinit",NULL);
	if(file)
		load_file(file,FALSE);
	g_free(file);

	if(infos) {
		geniusinfobox(infos->str);
		g_string_free(infos,TRUE);
		infos=NULL;
	}

	if(errors) {
		geniuserrorbox(errors->str);
		g_free(errors);
		errors=NULL;
	}

	for(;;) {
		char *str;
		rewind_file_info();
		do
			str = get_expression(&got_eof);
		while(interrupted);
		evalexp(str,NULL,rl_outstream,NULL,"= \e[01;34m",TRUE);
		g_free(str);
		fprintf(rl_outstream,"\e[0m");

		if(infos) {
			geniusinfobox(infos->str);
			g_string_free(infos,TRUE);
			infos=NULL;
		}

		if(errors) {
			geniuserrorbox(errors->str);
			g_string_free(errors,TRUE);
			errors=NULL;
		}

		if(got_eof) {
			fprintf(rl_outstream,"\n");
			got_eof = FALSE;
			break;
		}
	}


	return 0;
}
