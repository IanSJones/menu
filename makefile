TITLE = "MENU"

# SYS5	- System V
# BSD	- Berkely
OSVER = SYS5

# For the $MAIL (mail notification).
# On some systems the alarm() does not come into effect while in the middle
# of a getch().  The alarms get buffered up until the user hits a key and
# then all the bells notifying of mail goes off.  You should take out the
# -DALARM on the CFLAGS to get around this.
# Works ok on:		-DALARM
#	AT&T 3b1
#	AT&T 3b2
# Does not work on:
#	SUN
#	Arete
#	AT&T 3b15
#	AT&T 3b20
#	CT megaframe
#
#  -DSINGLE
#	Causes static references to terminal capabilities instead of dynamic 
#	references.  This results in smaller code. See /usr/include/term.h
#  -DLEXDEBUG
#	Only if you need more debugging for .DEFINE_SCREEN
#  -DYYDEBUG
#	Only if you need more debugging for .DEFINE_SCREEN
#  -DSUN
#	Sun compilation with 5bin/cc (SYS5)


INSTALLDIR = /usr/lbin
HELPDIR = /usr/lbin
MD = /usr/man/man1
INCDIR = ncurses:.
LIBDIR =

###  Various settings ###

###  Sys5 ###
# CFLAGS = -I$(INCDIR) -g -D$(OSVER) -DSINGLE -DLEXDEBUG -DYYDEBUG
# CFLAGS = +DA2.0N +DS2.0a -I$(INCDIR) -g -D$(OSVER) -DSINGLE -DHPBOX -DALARM
# 64 bit PA RISC 2.0 (PA8000) below


### Hp-UX ###
#
# HPUX 64 bit compile - NB libcur_colr won't link at 64 bit
# CFLAGS = -I$(INCDIR) +DA2.0W -D$(OSVER) -DSINGLE -DALARM -D_XOPEN_SOURCE_EXTENDED -g
CFLAGS = -I$(INCDIR) -D$(OSVER) -DSINGLE -DALARM -D_XOPEN_SOURCE_EXTENDED -g +DAportable
# LIBS = -lHcurses -ltermcap -lc -lm -ll -ly
### LIBS = -lc -lm -ll -lcurses -l:libcur_colr.sl
LIBS = -lc -lm -ll -lcurses
#
#############

### Linux ###
#
# CFLAGS = -I$(INCDIR) -D$(OSVER) -DSINGLE -DALARM -DLINUX -g
# LEXOPT=-l
# LIBS = -lcurses -lc -lm -lfl -lcrypt
#
#############

YFLAGS = -d

CFILES = AdjField.c       DateFun.c          DisPrmpt.c          FindSet.c              \
         GetSetLen.c      GetInput.c         IsDate.c            IsFldOk.c              \
         IsMask.c         IsRange.c          IsState.c           IsTime.c               \
	 ReDispFld.c      ScrnOut.c          ShowChar.c          ShowSet.c              \
         ShowHelp.c       popmenu.c          BuildMenu.c         checkmask.c            \
         doinsert.c       InitGetI.c         drawbox.c           RingMenu.c             \
         Main.c           LoadKeys.c         parsedrive.c        showdriver.c           \
         rundriver.c      ParseOpton.c       ParseBaner.c        ParseTitle.c           \
         ParseBox.c       ParseWindo.c       ParseLine.c         ParseComnt.c           \
         ParseUnix.c      ParseGname.c       ParseAuthr.c        ParseText.c            \
         ParseCur.c       ParseSpace.c       ParInclude.c        ParAssign.c            \
         ParseHelpFile.c  ShowOption.c       RunSystem.c         RunExit.c              \
         RunSetenv.c      RunMenu.c          RunPopMenu.c        RunGetI.c              \
         GetOption.c      RunPrevious.c      EndWindow.c         displaytxt.c           \
         SetTerm.c        SysInclude.c       systime.c           sysdate.c              \
         TrapSignal.c     checkmail.c        anymail.c           setenv.c               \
         strmatch.c       setvar.c           findfile.c          drawline.c             \
         initmenu.c       keyboard.c         runscreen.c         getval.c               \
         clean_menu.c     slength.c          substr.c            str_upper.c \
         propeller.c      ParseLineType.c    lexio.c             about.c                \
         can_i_see.c

CFILESUSER = AdjField.c       DateFun.c          DisPrmpt.c          FindSet.c              \
         GetSetLen.c      GetInputusermap.c  IsDate.c            IsFldOk.c              \
         IsMask.c         IsRange.c          IsState.c           IsTime.c               \
	 ReDispFld.c      ScrnOutusermap.c   ShowChar.c          ShowSet.c              \
         ShowHelp.c       popmenu.c          BuildMenu.c         checkmask.c            \
         doinsert.c       InitGetI.c         drawbox.c           RingMenu.c             \
         Mainusermap.c    LoadKeys.c         parsedrive.c        showdriver.c           \
         rundriverusermap.c ParseOpton.c     ParseBaner.c        ParseTitle.c           \
         ParseBox.c       ParseWindo.c       ParseLine.c         ParseComnt.c           \
         ParseUnix.c      ParseGname.c       ParseAuthr.c        ParseText.c            \
         ParseCur.c       ParseSpace.c       ParInclude.c        ParAssign.c            \
         ParseHelpFile.c  ShowOption.c       RunSystem.c         RunExit.c              \
         RunSetenv.c      RunMenu.c          RunPopMenu.c        RunGetI.c              \
         GetOptionusermap.c RunPrevious.c    EndWindow.c         displaytxt.c           \
         SetTerm.c        SysInclude.c       systime.c           sysdate.c              \
         TrapSignal.c     checkmail.c        anymail.c           setenv.c               \
         strmatch.c       setvar.c           findfile.c          drawline.c             \
         initmenu.c       keyboard.c         runscreen.c         getval.c               \
         clean_menu.c     slength.c          substr.c            str_upper.c   \
         propeller.c      ParseLineType.c    lexio.c             about.c                \
         can_i_seeusermap.c

LFILES = LexDeSrn.l lex_can_i_see.l
YFILES = ParseDeSrn.y parse_can_i_see.y

HFILES = menu.h terminal.h

MANPAGES = menu.1

OBJECTS = $(CFILES:.c=.o) $(YFILES:.y=.o) $(LFILES:.l=.o)
OBJECTSUSER = $(CFILESUSER:.c=.o) $(YFILES:.y=.o) $(LFILES:.l=.o)

all:	menu menuuser runrealid checkpass lock unlock

menu:	$(OBJECTS) 
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBDIR) $(LIBS) -o $@

menuuser:= CFLAGS = -I$(INCDIR) -D$(OSVER) -DSINGLE -DALARM -D_USERMAP

menuuser: $(OBJECTSUSER) 
	$(CC) $(CFLAGS) $(OBJECTSUSER) $(LIBDIR) $(LIBS) -o $@
#	$(CC) $(CFLAGS) -D_USERMAP $(OBJECTSUSER) $(LIBDIR) $(LIBS) -o $@

runrealid: runrealid.c
	$(CC) $(CFLAGS) runrealid.c -o $@

about:	about.o

about.o: about.c
	cc +DAportable -o about about.c
	cc +DAportable -c -D_ROUTINE about.c
	@echo
	@echo ============================================
	@echo Dont forget...
	@echo --------------
	@echo If youve changed the about routine
	@echo youll have to copy the about command
	@echo into /usr/local/bin on all machines.
	@echo ============================================
	@echo


#can_i_see: parse_can_i_see.o lexio.o lex_can_i_see.o can_i_see.o 

#can_i_see.o: parse_can_i_see.y lex_can_i_see.l can_i_see.c lexio.h can_i_see.h
can_i_see.o: parse_can_i_see.o lex_can_i_see.o can_i_see.c lexio.h can_i_see.h
	cc +DAportable -o can_i_see can_i_see.c parse_can_i_see.o lexio.o lex_can_i_see.o
	rm -f can_i_see.o
	cc +DAportable -c -D_ROUTINE can_i_see.c parse_can_i_see.o lexio.o lex_can_i_see.o
	@echo
	@echo ============================================
	@echo Dont forget...
	@echo --------------
	@echo If youve changed the can_i_see routine
	@echo youll have to copy the can_i_see command
	@echo into /usr/local/bin on all machines.
	@echo ============================================
	@echo

can_i_seeusermap.o: can_i_see.c lexio.h can_i_see.h parse_can_i_see.y lex_can_i_see.l
	cc +DAportable -c -ocan_i_seeusermap.o -D_ROUTINE $(CFLAGS) can_i_see.c parse_can_i_see.o lexio.o lex_can_i_see.o

checkpass: checkpass.c
	$(CC) $(CFLAGS) checkpass.c -o $@

lock: 	lock.c
	$(CC) $(CFLAGS) lock.c -o $@

unlock:	unlock.c
	$(CC) $(CFLAGS) unlock.c -o $@

install:
	strip menu
	cp menu $(INSTALLDIR)
	cp menu.hlp $(HELPDIR)
#	cp sample.m $(INSTALLDIR)
#	cp printers.m $(INSTALLDIR)
#	cp reportsrn.m $(INSTALLDIR)
#	cp menu.1 $(MD)
	strip runrealid
	cp runrealid $(INSTALLDIR)
	cp utilities.d/checkpass.d/checkpass $(INSTALLDIR)
#	cp utilities.d/checkpass.d/checkpass.1 $(MD)
	cp utilities.d/lock.d/lock $(INSTALLDIR)
	cp utilities.d/lock.d/unlock $(INSTALLDIR)
#	cp utilities.d/lock.d/lock.1 $(MD)
#	cp utilities.d/lock.d/unlock.1 $(MD)

lint:
	lint -D LINT -I $(INCDIR) -I /usr/5include $(CFILES) -l Hcurses -l c > menu.lint 

xref:
	cxref -o menu.xref -c -t -I$(INCDIR) $(CFILES) > menu.xref

flow:
	cflow -I$(INCDIR) $(CFILES) > menu.flow

clean:
	rm -f *.o core menu menu.lint menu.shar* menu.xref menu.flow \
	menu.shar.Z y.tab.h menu.cpio.Z runrealid menuuser
	rm -f lock unlock checkpass about can_i_see

#####
#####

Main.o:		Main.c menu.h popmenu.h
Mainusermap.o:	Main.c menu.h popmenu.h
		cc +DAportable -c -oMainusermap.o $(CFLAGS) Main.c
ParseOpton.o:	ParseOpton.c menu.h
ParseBaner.o:	ParseBaner.c menu.h
ParseBox.o:	ParseBox.c menu.h
ParseTitle.o:	ParseTitle.c menu.h
ParseLine.o:	ParseLine.c menu.h
ParseWindo.o:	ParseWindo.c menu.h
ParseComnt.o:	ParseComnt.c menu.h
ParseUnix.o:	ParseUnix.c menu.h
ParseGname.o:	ParseGname.c menu.h
ParseAuthr.o:	ParseAuthr.c menu.h
ParseText.o:	ParseText.c menu.h
ParseCur.o:	ParseCur.c menu.h
ParseLineType.o: ParseLineType.c menu.h
ParseSpace.o:	menu.h
ParInclude.o:	menu.h
ParAssign.o:	menu.h
ParseHelpFile.o: ParseHelpFile.c menu.h
ParseDeSrn.o: 	ParseDeSrn.y LexDeSrn.l menu.h
		yacc $(YFLAGS) ParseDeSrn.y
		cc $(CFLAGS) -c y.tab.c -o ParseDeSrn.o
#		mv y.tab.o ParseDeSrn.o
		mv y.tab.h LexDeSrn.h
		rm -f y.tab.c
parse_can_i_see.o: parse_can_i_see.y lex_can_i_see.l can_i_see.h lexio.h
		yacc -l -d -p ij parse_can_i_see.y
		cc +DAportable -c y.tab.c -o parse_can_i_see.o
#		mv y.tab.o parse_can_i_see.o
		mv y.tab.h lex_can_i_see.h
		rm -f y.tab.c	
RunSystem.o:	RunSystem.c menu.h
RunMenu.o:	RunMenu.c menu.h
RunPopMenu.o:	RunPopMenu.c menu.h
RunExit.o:	menu.h
RunSetenv.o:	menu.h
RunGetI.o:	menu.h
RunPrevious.o:	RunPrevious.c menu.h
ShowOption.o:	ShowOption.c menu.h
SysInclude.o:	SysInclude.c menu.h
getval.o:	getval.c menu.h
parsedrive.o:	parsedrive.c menu.h
showdriver.o:	showdriver.c menu.h 
rundriver.o:	rundriver.c menu.h terminal.h
rundriverusermap.o:	rundriver.c menu.h terminal.h
		cc -c -orundriverusermap.o $(CFLAGS) rundriver.c
LoadKeys.o:	LoadKeys.c menu.h
EndWindow.o:	EndWindow.c menu.h
GetOption.o:	menu.h terminal.h
GetOptionusermap.o:	menu.h terminal.h
		cc -c -oGetOptionusermap.o $(CFLAGS) GetOption.c
SetTerm.o:	menu.h
setvar.o:	menu.h
initmenu.o:	menu.h
keyboard.o:	keyboard.c menu.h
propeller.o:	propeller.c menu.h
runscreen.o:	menu.h terminal.h
LexDeSrn.o: 	LexDeSrn.l ParseDeSrn.y menu.h
		lex LexDeSrn.l
		cc -c $(CFLAGS) lex.yy.c
		mv lex.yy.o LexDeSrn.o
		rm -f lex.yy.c
		rm -f y.tab.h
lex_can_i_see.o: lex_can_i_see.l parse_can_i_see.y can_i_see.h lexio.h
		lex $(LEXOPT) -t lex_can_i_see.l | sed 's/yy/ij/g' > lex.yy.c
		cc -c $(CFLAGS) lex.yy.c
		mv lex.yy.o lex_can_i_see.o
		rm -f lex.yy.c
AdjField.o:	AdjField.c GetInput.h
DisPrmpt.o:	DisPrmpt.c GetInput.h
FindSet.o:	FindSet.c GetInput.h
GetSetLen.o:	GetSetLen.c GetInput.h
GetInput.o:	GetInput.c GetInput.h keys.h
GetInputusermap.o:	GetInput.c GetInput.h keys.h
		cc -c -oGetInputusermap.o $(CFLAGS) GetInput.c
IsDate.o:	IsDate.c GetInput.h
IsFldOk.o:	IsFldOk.c GetInput.h
IsMask.o:	IsMask.c GetInput.h
IsRange.o:	IsRange.c GetInput.h
IsTime.o:	IsTime.c GetInput.h
ReDispFld.o:	ReDispFld.c GetInput.h
ScrnOut.o:	ScrnOut.c GetInput.h
ScrnOutusermap.o:	ScrnOut.c GetInput.h
		cc -c -oScrnOutusermap.o $(CFLAGS) ScrnOut.c
ShowChar.o:	ShowChar.c GetInput.h
ShowHelp.o:	ShowHelp.c menu.h keys.h GetInput.h
ShowSet.o:	ShowSet.c GetInput.h
popmenu.o:	popmenu.c popmenu.h
clean_menu.o:	clean_menu.c menu.h popmenu.h
lexio.o:	lexio.c lexio.h
BuildMenu.o:	popmenu.h menu.h
