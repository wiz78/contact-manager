
CC 	= ppc-morphos-gcc
#CDEFS  = -DUSE_INLINE_STDARG
CFLAGS  = $(CDEFS) $(CWARN) -O2 -fomit-frame-pointer -mcpu=750 -mstring -mmultiple -noixemul -fstrict-aliasing -I../include/
LDFLAGS = -noixemul

#CWARN = -Wall -Wno-parentheses
CWARN = -Wall\
	-Wno-missing-braces\
	-Wno-parentheses\
        -Wno-switch\
        -Wstrict-prototypes\
	-Werror-implicit-function-declaration\
        -Wpointer-arith

OBJS = cm.o app.o win.o about.o parseArgs.o utils.o

CManager: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)
	ppc-morphos-strip $@

cm.o:        cm.c cm.h CManager_rev.h cm_protos.h
app.o:       app.c cm.h cm_protos.h
win.o:       win.c cm.h cm_protos.h
about.o:     about.c cm.h CManager_rev.h cm_protos.h
parseArgs.o: parseArgs.c cm.h CManager_rev.h cm_protos.h
utils.o:     utils.c cm.h cm_protos.h

