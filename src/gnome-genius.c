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
#include <config.h>
#include <gnome.h>
#include <gtk/gtk.h>

#include <string.h>
#include <stdlib.h>

#include "gnome-genius.h"
#include "calc.h"
#include "util.h"

/*Globals:*/

/*calculator state*/
calcstate_t curstate={
	256,
	12,
	FALSE,
	FALSE,
	FALSE
        };

extern calc_error_t error_num;
extern int got_eof;

/*static int base=10;*/
static GtkWidget *history; /*history list (CList) */
static GtkWidget *entry;
static GtkWidget *setupdialog = NULL;

static GList *answers=NULL; /*FIXME: this should be an array I guess*/

static GtkWidget *numframe, *sciframe,*optframe;

static char *errors=NULL;
static char *config_defaults[] = { "/genius/view/numpad=true",
	                           "/genius/view/scientific=false" };

static GdkColor rescol;

static void set_properties (void);

/* show/hide frame */
static void
showhide(GtkWidget * widget,gpointer * data)
{
	GtkWidget *frm=NULL;

	switch((long int)data) {
		case 1: frm=numframe; break;
		case 2: frm=sciframe; break;
		case 3: frm=optframe; break;
	}

	if(GTK_CHECK_MENU_ITEM(widget)->active)
		gtk_widget_show(frm);
	else
		gtk_widget_hide(frm);
}

/*get the string from entry and make a new string (g_malloc) */
/*PLUS ONE! .. the string can be appended by one character!*/
static char *
getentry(GtkWidget* e)
{
	char *pt,*text;

	pt=gtk_entry_get_text(GTK_ENTRY(e));
	text=(char *)g_malloc(strlen(pt)+2);
	strcpy(text,pt);
	return text;
}


/* add the key's label to entry in data */
static void
addkey(gchar c)
{
	char *t;
	int curpos;

	curpos=GTK_EDITABLE(entry)->current_pos;

	t=getentry(entry);
	shiftstr(&t[curpos],1);
	t[curpos]=c;
	gtk_entry_set_text(GTK_ENTRY(entry),t);
	gtk_entry_set_position(GTK_ENTRY(entry),curpos+1);
	g_free(t);
}

/* add the key's label to entry in data without borders */
static void
addkeycbnb(GtkWidget * widget, gpointer * data)
{
	char *p;
	int i;
	gtk_label_get(GTK_LABEL(GTK_BUTTON(widget)->child),&p);
	for(i=1;i<strlen(p)-1;i++)
		addkey(p[i]);
}

/* add the key's label to entry in data */
static void
addkeycb(GtkWidget * widget, gpointer * data)
{
	char *p;
	int i;
	gtk_label_get(GTK_LABEL(GTK_BUTTON(widget)->child),&p);
	for(i=0;i<strlen(p);i++)
		addkey(p[i]);
}

/*display a message in a messagebox*/
static void
geniuserrorbox(char *s)
{
	GtkWidget *mb;

	mb=gnome_message_box_new(s,GNOME_MESSAGE_BOX_ERROR,
				 GNOME_STOCK_BUTTON_OK,NULL);

	gtk_widget_show(mb);
}

/*get error message*/
static void
geniuserror(char *s)
{
	if(errors) {
		errors=my_realloc(errors,strlen(errors)+1,
			strlen(errors)+1+strlen(s)+1);
		strcat(errors,"\n");
		strcat(errors,s);
	} else {
		errors=g_malloc(strlen(s)+1);
		strcpy(errors,s);
	}
}

static void
history_select_row(GtkCList * clist,
		   gint row,
		   gint column,
		   GdkEventButton * event)
{
	char *t;

	t = (char *)g_list_nth(answers,row)->data;

	if(!t) return;

	gtk_entry_set_text(GTK_ENTRY(entry),t);
}

/* parse and evaluate the xpression and get answer and stick it down
  the history list*/
static void
dorun(GtkWidget * widget, gpointer * data)
{
	char *t[2];
	char *o[2];
	int newrow;
	static int width = 0;


	t[1]=getentry(entry);
	if(!t[1] || t[1][0]=='\0')
		return;
	t[1]=addparenth(t[1]); /*add missing parenthesis*/
	o[1]=evalexp(t[1],NULL,NULL,NULL,curstate,geniuserror,FALSE);

	if(errors) {
		geniuserrorbox(errors);
		g_free(errors);
		errors=NULL;
	}
	
	if(got_eof)
		gtk_main_quit();
	if(!o[1])
		return;

	answers = g_list_append(answers,g_strdup(t[1]));
	t[0]="";
	gtk_clist_append(GTK_CLIST(history),t);

	if(width<gdk_string_width(GTK_WIDGET(history)->style->font,t[1]))
		width = gdk_string_width(GTK_WIDGET(history)->style->font,t[1]);
	g_free(t[1]);


	answers = g_list_append(answers,g_strdup(o[1]));
	o[0]="=";
	newrow=gtk_clist_append(GTK_CLIST(history),o);
	gtk_clist_set_background(GTK_CLIST(history),newrow,&rescol);

	if(width<gdk_string_width(GTK_WIDGET(history)->style->font,o[1]))
		width = gdk_string_width(GTK_WIDGET(history)->style->font,o[1]);
	
	g_free(o[1]);

	gtk_clist_moveto(GTK_CLIST(history),newrow-1,-1,0.0,1.0);
	gtk_clist_set_column_width(GTK_CLIST(history),1,width);
	gtk_clist_set_column_width(GTK_CLIST(history),0,
		gdk_string_width(GTK_WIDGET(history)->style->font,"= "));

	/*clear the input box*/
	gtk_entry_set_text(GTK_ENTRY(entry),"");
}

/*about box*/
static void
aboutcb(GtkWidget * widget, gpointer * data)
{
	GtkWidget *mb;
	gchar *authors[] = {
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
			   
	gtk_widget_show(mb);
}

/* quit */
static void
quitapp(GtkWidget * widget, gpointer * data)
{
	set_properties();
	gtk_main_quit();
}

/*exact answer callback*/
static void
exactanscb(GtkWidget * widget, gpointer * data)
{
	if(GTK_TOGGLE_BUTTON(widget)->active)
		*(int *)data=0;
	else
		*(int *)data=12;

	gnome_property_box_changed(GNOME_PROPERTY_BOX(setupdialog));
}

/*option callback*/
static void
optioncb(GtkWidget * widget, gpointer * data)
{
	if(GTK_TOGGLE_BUTTON(widget)->active)
		*(int *)data=TRUE;
	else
		*(int *)data=FALSE;
	
	gnome_property_box_changed(GNOME_PROPERTY_BOX(setupdialog));
}

/*keypress event handler*/
static gint
windowkeypress(GtkWidget *widget, GdkEventKey *event)
{
	char key=*(event->string);

	/*FIXME: should use accelerators for this!*/
	if(key==3 || key==24) /* ^C || ^X */
		quitapp(NULL,NULL);
	else if(!GTK_WIDGET_HAS_FOCUS(entry) &&
		strchr("\t\v ",key)==NULL)
		gtk_widget_grab_focus(entry);

	if(key=='\r' ||
		key=='\n')
		dorun(NULL,NULL);

	return TRUE;
}

static gint 
setupdialog_destroy(GtkWidget *widget, gpointer data)
{
	if (setupdialog) {
		setupdialog = NULL;
	}
	return FALSE; /* We didn't handle the event; PropertyBox should
			 do the destroy. */
}

static void
do_setup(GtkWidget *widget, gint page, gpointer data)
{
  if (page == -1) {     /* Just finished global apply */
    curstate = *((calcstate_t *) data);
  }
}

static gint
destroy_setup(GtkWidget *widget, gpointer data)
{
	setupdialog = NULL;
	return FALSE;
}

static void
setup_calc(GtkWidget *widget, gpointer data)
{
	GtkWidget *all_boxes;
	GtkWidget *box, *boxt;
	GtkWidget *w, *wt;

	static calcstate_t tmpstate;
	
	if (setupdialog) {
		gdk_window_raise(GTK_WIDGET(setupdialog)->window);
		return;
	}
	
	tmpstate = curstate;
	
	setupdialog = gnome_property_box_new();
	
	GTK_WINDOW(setupdialog)->position = GTK_WIN_POS_MOUSE;
	gtk_window_set_title(GTK_WINDOW(setupdialog), _("GnomENIUS Calculator setup"));
	gtk_signal_connect(GTK_OBJECT(setupdialog),
			   "delete_event",
			   GTK_SIGNAL_FUNC(setupdialog_destroy),
			   0);
	
	all_boxes = gtk_vbox_new(FALSE, GNOME_PAD);
	gtk_widget_show(all_boxes);
	gnome_property_box_append_page(GNOME_PROPERTY_BOX(setupdialog),
				       all_boxes,
				       gtk_label_new(_("General")));

	/*options area*/
	optframe=gtk_frame_new(_("Preferences"));
	box=gtk_vbox_new(FALSE,0);
	boxt=gtk_hbox_new(FALSE,0);
	
	boxt = gtk_table_new(2, 2, TRUE);
	gtk_box_pack_start(GTK_BOX(box),boxt,TRUE,TRUE,5);
	gtk_widget_show(boxt);
	w=gtk_check_button_new_with_label(_("Full (exact) answers"));
	gtk_signal_connect(GTK_OBJECT(w), "toggled",
		   GTK_SIGNAL_FUNC(exactanscb),&tmpstate.max_digits);
	gtk_table_attach_defaults(GTK_TABLE(boxt), GTK_WIDGET(w), 0, 1, 0, 1);
	gtk_widget_show(w);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(w),
				    (tmpstate.max_digits == 0) ?
				    TRUE : FALSE);
	
	w=gtk_check_button_new_with_label(
		_("Make floats integers (e.g. 10.0=10)"));
	gtk_signal_connect(GTK_OBJECT(w), "toggled",
		   GTK_SIGNAL_FUNC(optioncb),
		   (gpointer *)&tmpstate.make_floats_ints);
	gtk_table_attach_defaults(GTK_TABLE(boxt), GTK_WIDGET(w), 1, 2, 0, 1);
	gtk_widget_show(w);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(w), 
				    tmpstate.make_floats_ints);

	w=gtk_check_button_new_with_label(_("Results as floats"));
	gtk_signal_connect(GTK_OBJECT(w), "toggled",
		   GTK_SIGNAL_FUNC(optioncb),
		   (gpointer *)&tmpstate.results_as_floats);
	gtk_table_attach_defaults(GTK_TABLE(boxt), GTK_WIDGET(w), 0, 1, 1, 2);
	gtk_widget_show(w);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(w), 
				    tmpstate.results_as_floats);
	
	w=gtk_check_button_new_with_label(_("Floats in scientific notation"));
	gtk_signal_connect(GTK_OBJECT(w), "toggled",
		   GTK_SIGNAL_FUNC(optioncb),
		   (gpointer *)&tmpstate.scientific_notation);
	gtk_table_attach_defaults(GTK_TABLE(boxt), GTK_WIDGET(w), 1, 2, 1, 2);
	gtk_widget_show(w);
	gtk_toggle_button_set_state(GTK_TOGGLE_BUTTON(w), 
				    tmpstate.scientific_notation);

	gtk_container_add(GTK_CONTAINER(optframe),box);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(all_boxes),optframe,FALSE,FALSE,5);
	gtk_widget_show(optframe);
		
	gtk_signal_connect(GTK_OBJECT(setupdialog), "apply",
			   GTK_SIGNAL_FUNC(do_setup), (gpointer *) &tmpstate);	
	gtk_signal_connect(GTK_OBJECT(setupdialog), "destroy",
			   GTK_SIGNAL_FUNC(destroy_setup), NULL);
	gtk_widget_show(all_boxes);
	gtk_widget_show(setupdialog);
}

static GnomeUIInfo file_menu[] = {
	GNOMEUIINFO_MENU_EXIT_ITEM(quitapp,NULL),
	GNOMEUIINFO_END,
};

static GnomeUIInfo settings_menu[] = {  
	GNOMEUIINFO_TOGGLEITEM_DATA(N_("Show _Numpad"),N_("Show the number pad"),showhide, (gpointer)1, NULL),
	GNOMEUIINFO_TOGGLEITEM_DATA(N_("Show _Scientific"),N_("Show the scientific pad"),showhide, (gpointer)2, NULL),
	GNOMEUIINFO_MENU_PREFERENCES_ITEM(setup_calc,NULL),
	GNOMEUIINFO_END,
};

static GnomeUIInfo help_menu[] = {  
	GNOMEUIINFO_HELP("genius"),
	GNOMEUIINFO_MENU_ABOUT_ITEM(aboutcb,NULL),
	GNOMEUIINFO_END,
};
  
static GnomeUIInfo genius_menu[] = {
	GNOMEUIINFO_MENU_FILE_TREE(file_menu),
	GNOMEUIINFO_MENU_SETTINGS_TREE(settings_menu),
	GNOMEUIINFO_MENU_HELP_TREE(help_menu),
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
	gboolean tmp;
	gchar buf[256];
        long i;
	
	for (i = 0; i < 2; i++) {
		tmp = gnome_config_get_bool(config_defaults[i]);
		GTK_CHECK_MENU_ITEM (settings_menu[i].widget)->active = tmp;
		showhide(GTK_WIDGET(settings_menu[i].widget), (gpointer *) (i + 1));
	}
	
	g_snprintf(buf,256,"/genius/properties/max_digits=%s",
		   (curstate.max_digits == 0)?"true":"false");
	curstate.max_digits = (gnome_config_get_bool(buf))? 0 : 12;
	g_snprintf(buf,256,"/genius/properties/make_floats_ints=%s",
		   curstate.make_floats_ints?"true":"false");
	curstate.make_floats_ints = gnome_config_get_bool(buf);
	g_snprintf(buf,256,"/genius/properties/results_as_floats=%s",
		   curstate.results_as_floats?"true":"false");
	curstate.results_as_floats = gnome_config_get_bool(buf);
	g_snprintf(buf,256,"/genius/properties/scientific_notation=%s",
		   curstate.scientific_notation?"true":"false");
	curstate.scientific_notation = gnome_config_get_bool(buf);
}

static void
set_properties (void)
{
	gnome_config_set_bool("/genius/properties/max_digits", 
			      (curstate.max_digits == 0) ? TRUE : FALSE);
	gnome_config_set_bool("/genius/properties/make_floats_ints", 
			     curstate.make_floats_ints);
	gnome_config_set_bool("/genius/properties/results_as_floats",
			     curstate.results_as_floats);
	gnome_config_set_bool("/genius/properties/scientific_notation",
			     curstate.scientific_notation);
	
	if(GTK_CHECK_MENU_ITEM(settings_menu[0].widget)->active) 
		gnome_config_set_bool("/genius/view/numpad",  TRUE);
	else
		gnome_config_set_bool("/genius/view/numpad",  FALSE);
	
	if(GTK_CHECK_MENU_ITEM(settings_menu[1].widget)->active)
		gnome_config_set_bool("/genius/view/scientific",  TRUE);
	else
		gnome_config_set_bool("/genius/view/scientific",  FALSE);
	
	gnome_config_sync();
}

/*
 * Kind of a long main but all the gui stuff is here and that I guess
 * should be kept in one place
 */
int
main(int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *w; /*widget temp variable*/
	GtkWidget *box,*boxm,*boxh,*boxt;
	GtkWidget *frame;
	GtkWidget *table;
	GtkTooltips *tips;
	char *file;
	FILE *fp;


	/*numpad button labels*/
	char *numpad[5][4]={
		{" ( "," ) "," ^ "," % "},
		{" 7 "," 8 "," 9 "," / "},
		{" 4 "," 5 "," 6 "," * "},
		{" 1 "," 2 "," 3 "," - "},
		{" 0 "," . "," ~ "," + "}
	};
	
	/*scientific*/
	char *scientific[4][3]={
		{" sin "," cos "," tan "},
		{" pi "," e ", NULL},
		{" is_complex "," Re "," Im "},
		{" round "," sqrt ", " ! "}
	};
	int x,y;

	bindtextdomain(PACKAGE,GNOMELOCALEDIR);
	textdomain(PACKAGE);

	gnome_init("genius", NULL, argc, argv);
	
        /*set up the top level window*/
	window=create_main_window();

	/*set up the tooltips*/
	tips=gtk_tooltips_new();

	/*the main box to put everything in*/
	boxm=gtk_vbox_new(FALSE,0);


	boxh=gtk_hbox_new(FALSE,0);
	
	frame=gtk_frame_new(_("Display"));
	box=gtk_vbox_new(FALSE,0);

	w = gtk_scrolled_window_new(NULL,NULL);
	gtk_widget_show(w);
	gtk_widget_set_usize(w,200,100);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(w),
				       GTK_POLICY_AUTOMATIC,
				       GTK_POLICY_ALWAYS);
	gtk_box_pack_start(GTK_BOX(box),w,TRUE,TRUE,0);
	history=gtk_clist_new(2);
	gtk_widget_show(history);
	gtk_container_add(GTK_CONTAINER(w),history);
	gtk_signal_connect(GTK_OBJECT(history), "select_row",
                      GTK_SIGNAL_FUNC(history_select_row), NULL);

	boxt=gtk_hbox_new(FALSE,0);
	entry=gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(boxt),entry,TRUE,TRUE,0);
	gtk_widget_show(entry);
	w=gtk_button_new_with_label(" = ");
	gtk_signal_connect(GTK_OBJECT(w), "clicked",
		GTK_SIGNAL_FUNC(dorun), entry);
	gtk_box_pack_start(GTK_BOX(boxt),w,FALSE,FALSE,0);
	gtk_widget_show(w);
	gtk_box_pack_start(GTK_BOX(box),boxt,FALSE,FALSE,0);
	gtk_widget_show(boxt);



	gtk_container_add(GTK_CONTAINER(frame),box);
	gtk_widget_show(box);
	gtk_box_pack_start(GTK_BOX(boxh),frame,TRUE,TRUE,0);
	gtk_widget_show(frame);




	/*numpad*/
	frame=gtk_frame_new(_("Numpad"));
	table=gtk_table_new(7,6,0);
	gtk_table_set_col_spacings(GTK_TABLE(table),5);
	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	for(x=0;x<4;x++) {
		for(y=0;y<5;y++) {
			w=gtk_button_new_with_label(numpad[y][x]);
			if(numpad[y][x][1]>='0' ||
			   numpad[y][x][1]<='9')  {
				gtk_signal_connect(GTK_OBJECT(w), "clicked",
						   GTK_SIGNAL_FUNC(addkeycbnb), NULL);
			} else {
				gtk_signal_connect(GTK_OBJECT(w), "clicked",
						   GTK_SIGNAL_FUNC(addkeycb), NULL);
			}
			gtk_table_attach_defaults(GTK_TABLE(table),w,
				x+1,x+2,y+1,y+2);
			gtk_widget_show(w);
		}
	}


	gtk_container_add(GTK_CONTAINER(frame),table);
	gtk_widget_show(table);
	
	numframe = gtk_alignment_new (0.0, 1.0, 0.0, 0.0);
	gtk_container_add (GTK_CONTAINER (numframe), frame);
	gtk_widget_show (frame);
	gtk_box_pack_start(GTK_BOX(boxh),numframe, FALSE, FALSE,0);

	/*scientific*/
	sciframe=gtk_frame_new(_("Scientific"));
	gtk_box_pack_start(GTK_BOX(boxh),sciframe,FALSE,FALSE,0);

	table=gtk_table_new(6,5,0);
	gtk_table_set_col_spacings(GTK_TABLE(table),5);
	gtk_table_set_row_spacings(GTK_TABLE(table),5);
	for(x=0;x<3;x++) {
		for(y=0;y<4;y++) {
			if(scientific[y][x]) {
				w=gtk_button_new_with_label(scientific[y][x]);
				gtk_signal_connect(GTK_OBJECT(w), "clicked",
						   GTK_SIGNAL_FUNC(addkeycb), NULL);
				gtk_table_attach_defaults(GTK_TABLE(table),w,
							  x+1,x+2,y+1,y+2);
				gtk_widget_show(w);
			}
		}
	}
	gtk_container_add(GTK_CONTAINER(sciframe),table);
	gtk_widget_show(table);
	

	gtk_box_pack_start(GTK_BOX(boxm),boxh,TRUE,TRUE,5);
	gtk_widget_show(boxh);

	/*set up the keypress handler*/
	gtk_signal_connect(GTK_OBJECT(window), "key_press_event",
                      GTK_SIGNAL_FUNC(windowkeypress), NULL);
	gtk_widget_set_events(window, GDK_KEY_PRESS_MASK);

	/*set up the menu*/
        gnome_app_create_menus(GNOME_APP(window), genius_menu);
	
	/*setup appbar*/
	w = gnome_appbar_new(FALSE, TRUE, GNOME_PREFERENCES_USER);
	gnome_app_set_statusbar(GNOME_APP(window), w);
	gtk_widget_show(w);

	gnome_app_install_menu_hints(GNOME_APP(window),
				     genius_menu);


	/*read gnome_config parameters */
	
	/*set up the main window*/
	gnome_app_set_contents(GNOME_APP(window), boxm);
	gtk_container_border_width(
		GTK_CONTAINER(GNOME_APP(window)->contents),5);

	get_properties();

	gtk_widget_show(boxm);
	gtk_widget_show(window);

	gdk_color_white(gtk_widget_get_colormap(history),&rescol);
	
	file = g_strconcat(LIBRARY_DIR,"/lib.gel",NULL);
	/*read standard files and init scripts*/
	if((fp = fopen(file,"r"))) {
		while(1) {
			g_free(evalexp(NULL,fp,NULL,NULL,curstate,geniuserror,FALSE));
			if(got_eof) {
				got_eof = FALSE;
				break;
			}
		}
		fclose(fp);
	}
	g_free(file);

	/*try the library file in the current directory*/
	if((fp = fopen("lib.gel","r"))) {
		while(1) {
			g_free(evalexp(NULL,fp,NULL,NULL,curstate,geniuserror,FALSE));
			if(got_eof) {
				got_eof = FALSE;
				break;
			}
		}
		fclose(fp);
	}
	
	file = g_strconcat(getenv("HOME"),"/.geniusinit",NULL);

	fp = NULL;
	if(file && (fp = fopen(file,"r"))) {
		while(1) {
			g_free(evalexp(NULL,fp,NULL,NULL,curstate,geniuserror,FALSE));
			if(got_eof) {
				got_eof = FALSE;
				break;
			}
		}
	}
	if(fp) fclose(fp);

	if(errors) {
		geniuserrorbox(errors);
		g_free(errors);
		errors=NULL;
	}

	gtk_widget_grab_focus(entry);
	gtk_main();

	return 0;
}
