#include "config.h"
#include <stdio.h>
#include <gtk/gtk.h>
#include "plug_api.h"
#include "calc.h"

static void
open(void)
{
	printf("testplugin open()\n");
	puts("trying to evaluate 2+2");
	
	evalexp("2+2",NULL,stdout,NULL,NULL,TRUE);
	
	puts("ending...");
}

static int
save_state(char *prefix)
{
	printf("testplugin save_state(%s)\n",prefix);
	return FALSE;
}

static void
restore_state(char *prefix)
{
	printf("testplugin restore_state(%s)\n",prefix);
}

static PluginInfo info = {
	open,
	save_state,
	restore_state
};

PluginInfo *
init_func(void)
{
	return &info;
}
