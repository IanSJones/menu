/*
**	Type of allowable characters
*/

#define	ALPHANUM	' '
#define	ALPHA		'A'
#define	NUMERIC		'9'
#define	SET		'E'
#define	MENU		'M'
#define	LOWER		'L'
#define	LOWER_AN	'W'
#define	UPPER		'U'
#define	UPPER_AN	'V'
#define	HEX		'H'
#define	STATE		'S'
#define	ZIP		'Z'
#define	DATE		'D'
#define	TIME		'T'
#define	PROTECT		'P'
#define FILE_MANAGER	'F'

/*
**	Adjust/Fill
*/

#define	NOFILL		' '
#define	RTADJ_ZFILL	'Z'
#define	RTADJ_BFILL	'B'
#define	LFADJ_ZFILL	'z'
#define	LFADJ_BFILL	'b'

/*
**	'C' field types
*/

#define CHAR		'0'
#define INT			'1'
#define SHORT		'2'
#define LONG		'3'
#define DOUBLE		'4'
#define MONEY		'M'
#define FLOAT		'5'

/*
**	Union to take care of a field that could be any valid
**		'c' field type.
*/

typedef union FldUnion {
	long	*longVal ;
	double	*doubleVal ;
	float	*floatVal ;
	int	*intVal ;
	short	*shortVal ;
	char	*strVal ;
} *FldUnPointer ;


#define AUTONEXT	TRUE
#define NOAUTO		FALSE

#define MUSTENTER	TRUE
#define NOMUST		FALSE

#define NOHELP		""
#define NORANGE		NULL
#define NOMSG		NULL
#define NOMASK		NULL
#define NOTAG		""
#define	A_SAME		-1		/* same as what's on the screen */


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
#ifndef KEY_RETURN
#   define KEY_RETURN	'\r'
#endif
#ifndef KEY_BEG
#   define KEY_BEG	0542
#endif
#ifndef KEY_END
#   define KEY_END	0550
#endif
#ifndef	KEY_SAVE
#   define KEY_SAVE	0571
#endif
#ifndef KEY_PRINT
#   define KEY_PRINT 	0532
#endif
#ifndef	KEY_HELP
#   define KEY_HELP 	0553
#endif
#ifndef	KEY_REFRESH
#   define KEY_REFRESH 	0565
#endif
#ifndef	KEY_TAB
#   define KEY_TAB 	'\t'
#endif
#ifndef	KEY_BTAB
#   define KEY_BTAB 	0541
#endif
#ifndef	KEY_CANCEL
#   define KEY_CANCEL 	0543
#endif
#ifndef	KEY_ACCEPT
#   define KEY_ACCEPT 	1000
#endif
#ifndef	KEY_TOC
#   define KEY_TOC 	1001
#endif
#ifndef KEY_EXITMENU
#   define KEY_EXITMENU	1003
#endif

#define NOKEY		'\0'
#define KEY_CTLA	'\001'
#define KEY_CTLB	'\002'
#define KEY_CTLC	'\003'
#define KEY_CTLD	'\004'
#define KEY_CTLE	'\005'
#define KEY_CTLF	'\006'
#define KEY_CTLG	'\007'
#define KEY_CTLH	'\010'
#define KEY_CTLI	'\011'
#define KEY_CTLJ	'\012'
#define KEY_CTLK	'\013'
#define KEY_CTLL	'\014'
#define KEY_CTLM	'\015'
#define KEY_CTLN	'\016'
#define KEY_CTLO	'\017'
#define KEY_CTLP	'\020'
#define KEY_CTLQ	'\021'
#define KEY_CTLR	'\022'
#define KEY_CTLS	'\023'
#define KEY_CTLT	'\024'
#define KEY_CTLU	'\025'
#define KEY_CTLV	'\026'
#define KEY_CTLW	'\027'
#define KEY_CTLX	'\030'
#define KEY_CTLY	'\031'
#define KEY_CTLZ	'\032'

/*
**  Macros
*/
#define wattrOn(a, b)	wattrset(a, b)
#define wattrOff(a)	wattrset(a, 0)
#ifndef BELL
#define	BELL	printf ("%c", 7)
#endif

#define	CLEARROW(row)	{ \
				int	col; \
				int	tmprow; \
				tmprow = ((row%100) == row) ? row : row/100; \
				col = ((row%100) == row) ? 0 : row%100; \
				for (; col<=COLS-2; col++) \
					mvwaddch (stdscr, tmprow,col, ' '); \
			}
