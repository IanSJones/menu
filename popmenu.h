#include <curses.h>

#define MAXITEMS	2048		/* max options in popmenu */
#define	MAXMENUS	100		/* max number of popup menus */

#ifdef	MENU
#undef	MENU
#endif

typedef struct	_menu
{
	int	MenuId;			/* numerical menu identifier */
	int	row;
	int	col;
	char	*title ;		/* menu title */
	char	*helpfile;		/* helpfile */
	int	winSize ;		/* pop up window length */
	WINDOW	*win;
	int	NumItems;		/* number of items in menu */
	int	width;			/* largest item */
	char	*items[MAXITEMS+1];
	int	ActiveItem;		/* last item selected */
} MENU ;

#ifndef KEY_F1
#   define KEY_F1	0411
#endif
#ifndef KEY_F2
#   define KEY_F2	0412
#endif
#ifndef KEY_F3
#   define KEY_F3	0413
#endif
#ifndef KEY_F4
#   define KEY_F4	0414
#endif
#ifndef KEY_F5
#   define KEY_F5	0415
#endif
#ifndef KEY_F6
#   define KEY_F6	0416
#endif
#ifndef KEY_F7
#   define KEY_F7	0417
#endif
#ifndef KEY_F8
#   define KEY_F8	0420
#endif
#ifndef KEY_F9
#   define KEY_F9	0421
#endif
#ifndef KEY_F10
#   define KEY_F10	0422
#endif
#ifndef KEY_SHIFT_F1
#   define KEY_SHIFT_F1	0435
#endif
#ifndef KEY_SHIFT_F2
#   define KEY_SHIFT_F2	0436
#endif
#ifndef KEY_SHIFT_F3
#   define KEY_SHIFT_F3	0437
#endif
#ifndef KEY_SHIFT_F4
#   define KEY_SHIFT_F4	0440
#endif
#ifndef KEY_SHIFT_F5
#   define KEY_SHIFT_F5	0441
#endif
#ifndef KEY_SHIFT_F6
#   define KEY_SHIFT_F6	0442
#endif
#ifndef KEY_SHIFT_F7
#   define KEY_SHIFT_F7	0443
#endif
#ifndef KEY_SHIFT_F8
#   define KEY_SHIFT_F8	0444
#endif
#ifndef KEY_SHIFT_F9
#   define KEY_SHIFT_F9	0445
#endif
#ifndef KEY_SHIFT_F10
#   define KEY_SHIFT_F10	0446
#endif
