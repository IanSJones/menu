#include	<curses.h>
#include	"menu.h"
#include	"popmenu.h"


extern	int	debug;
extern	MENU	*THEmenu[];
extern	int	midx;

clean_menu (menu)
	struct MenuInfo	*menu;
{
	int		i,j;


	/*
	** Just to keep things simple, lets start from scratch on the
	** next menu selected.
	*/

	/* free options */
      	for (i = 0; i < menu->optioncount; i++)
	{
		if (debug>8) fprintf(stderr, "\nclean_menu.c:\tAbout to free bytes from menu->option[%d] (%p)", i, menu->option[i]);
       		free (menu->option[i]);
	}

      	/* free screens */
	for (i = 0; menu->srn[i] != (struct ScreenInfo *)NULL  &&  
	     i <= MAXSCREENS; i++)
	{
		if (menu->srn[i]->fielddefaults != (char *)NULL)
		{
			if (debug>8) fprintf(stderr, "\nclean_menu.c:\tAbout to free bytes from fielddefaults for %s (%p)", menu->srn[i]->name, menu->srn[i]->fielddefaults);
			free (menu->srn[i]->fielddefaults);
		}
		/* free screen fields */
		for (j = 0; 
		     menu->srn[i]->field[j] != (struct FieldInfo *)NULL &&
		     j <= MAXFIELDS; j++)
		{
			if (debug>8) fprintf(stderr, "\nclean_menu.c:\tAbout to free bytes from field[%d] for %s, (%p)", j, menu->srn[i]->name, menu->srn[i]->field[j]);
			free (menu->srn[i]->field[j]);
		}
		if (debug>8) fprintf(stderr, "\nclean_menu.c:\tAbout to free bytes from screen %d \"%s\" (%p)", i, menu->srn[i]->name, menu->srn[i]);
		free (menu->srn[i]);
		menu->srn[i] = (struct ScreenInfo *)NULL;
	}

	/* free popmenu space */
	for (i=0; i<=MAXMENUS; i++)
	{
		if (THEmenu[i] != '\0')
		{
			if (debug>8) fprintf(stderr, "\nclean_menu.c:\tAbout to free bytes from popmenu memory (%p)", THEmenu[i]);
			free(THEmenu[i]);
			/* THEmenu[i]=(struct MENU *)NULL; */
			THEmenu[i]=0;
		}
	}

	/* midx is the 'have we created at least one popmenu yet?' value */
	if (debug) fprintf(stderr, "\nclean_menu.c:\tresetting midx", THEmenu[i]);
	midx=-1;
}
