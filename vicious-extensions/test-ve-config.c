#include <config.h>
#include <glib.h>
#include <stdio.h>
#include "ve-config.h"

/* tests done ona gdm.conf like file */

int
main (int argc, char *argv[])
{
	GList *list;
	VeConfig *config = ve_config_new (argv[1]);

	printf ("Get all sections:\n");
	list = ve_config_get_sections (config);
	while (list != NULL) {
		printf ("%s\n", (char *)list->data);
		list = list->next;
	}
	printf ("\n");

	printf ("Get all keys in section 'greeter':\n");
	list = ve_config_get_keys (config, "greeter");
	while (list != NULL) {
		printf ("%s\n", (char *)list->data);
		list = list->next;
	}
	printf ("\n");

	printf ("daemon/AutomaticLoginEnable=%d\n",
		 (int) ve_config_get_bool
		 (config, "daemon/AutomaticLoginEnable=false"));
	printf ("gui/MaxIconHeight=%d\n",
		 ve_config_get_int
		 (config, "gui/MaxIconHeight=99"));
	printf ("gui/MaxIconWidth=%d\n",
		 ve_config_get_int
		 (config, "gui/MaxIconWidth"));
	printf ("test=%s\n",
		 ve_config_get_string
		 (config, "test=test"));
	printf ("greeter/Welcome=%s\n",
		 ve_config_get_translated_string
		 (config, "greeter/Welcome=test"));
	ve_config_set_string (config, "greeter/DefaultLocale=en_US", "BAH!");
	ve_config_set_string (config, "greeter/DefaultLocale1=en_US", "BAFAN!");
	ve_config_set_string (config, "greeter/DefaultLocale2", "BAFAN2!");
	ve_config_set_string (config, "test=gogo", "bebe");
	ve_config_set_int (config, "greeter/foo", 12);
	ve_config_set_int (config, "greeter/bar=1", 13);
	ve_config_set_int (config, "queque/lolo=1", 14);
	ve_config_set_bool (config, "queque/quaqua", FALSE);
	ve_config_save (config, TRUE /* force */);
	ve_config_destroy (config);
	return 0;
}
