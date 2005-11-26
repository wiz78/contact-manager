
TARGET  = CManager.mcc
DEST    = SYS:Classes/MUI

CC 	= ppc-morphos-gcc
CDEFS   = -DUSE_INLINE_STDARG
CFLAGS  = $(CDEFS) $(CWARN) -O2 -fomit-frame-pointer -mcpu=750 -mmultiple -noixemul -fstrict-aliasing -I../include/
LDFLAGS = -noixemul

CWARN   = -Wall\
  	  -Wno-missing-braces\
  	  -Wno-parentheses\
          -Wno-switch\
          -Wstrict-prototypes\
  	  -Werror-implicit-function-declaration\
          -Wpointer-arith

OBJS = libinit.o init.o loc.o utils.o options.o lamp.o popph.o card.o prefs.o search.o login.o\
entrylist.o grouplist.o dial.o picmos.o delwarning.o rootscrollgroup.o rootstring.o rootscroll.o\
rootbar.o qstring.o popupcodesets.o popasl.o editWin.o editUser.o editWWW.o editFTP.o editIRC.o\
minimail.o editTelnet.o import.o export.o matherwin.o mcc.o

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -nostartfiles -o $@ $(OBJS)
	ppc-morphos-strip $@

install:
	@copy $(TARGET) $(DEST)
	@delete $(TARGET)
	@avail flush

libinit.o:         libinit.c class.h CManager.mcc_rev.h base.h class_protos.h
init.o:            init.c class.h base.h class_protos.h
loc.o:             loc.c class.h base.h class_protos.h
utils.o:           utils.c class.h base.h class_protos.h
options.o:         options.c class.h base.h class_protos.h
lamp.o:            lamp.c class.h base.h class_protos.h
popph.o:           popph.c class.h base.h class_protos.h
card.o:            card.c class.h base.h class_protos.h
prefs.o:           prefs.c class.h base.h class_protos.h
search.o:          search.c class.h base.h class_protos.h
login.o:           login.c class.h base.h class_protos.h
entrylist.o:       entrylist.c class.h base.h class_protos.h
grouplist.o:       grouplist.c class.h base.h class_protos.h
dial.o:            dial.c class.h base.h class_protos.h
picmos.o:          picmos.c class.h base.h class_protos.h
minimail.o:        minimail.c class.h base.h class_protos.h
delwarning.o:      delwarning.c class.h base.h class_protos.h
rootScrollgroup.o: rootScrollgroup.c class.h base.h class_protos.h
rootString.o:      rootString.c class.h base.h class_protos.h
rootScroll.o:      rootScroll.c class.h base.h class_protos.h
rootbar.o:         rootbar.c class.h base.h class_protos.h
qstring.o:         qstring.c class.h base.h class_protos.h
popupcodesets.o:   popupcodesets.c class.h base.h class_protos.h
popasl.o:          popasl.c class.h base.h class_protos.h
editWin.o:         editWin.c class.h base.h class_protos.h
editUser.o:        editUser.c class.h base.h class_protos.h
editWWW.o:         editWWW.c class.h base.h class_protos.h
editFTP.o:         editFTP.c class.h base.h class_protos.h
editIRC.o:         editIRC.c class.h base.h class_protos.h
editTelnet.o:      editTelnet.c class.h base.h class_protos.h
import.o:          import.c class.h base.h class_protos.h
export.o:          export.c class.h base.h class_protos.h
matherwin.o:       matherwin.c class.h base.h class_protos.h
mcc.o:             mcc.c class.h base.h class_protos.h

