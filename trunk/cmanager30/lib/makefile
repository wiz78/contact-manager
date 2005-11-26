TARGET  = CManager.library
DEST    = LIBS:

CC 	= ppc-morphos-gcc
#CDEFS   = -DUSE_INLINE_STDARG
CDEFS =
#CFLAGS  = $(CDEFS) $(CWARN) -noixemul -O2 -fomit-frame-pointer -I../include/
CFLAGS  = $(CDEFS) $(CWARN) -O2 -fomit-frame-pointer -mcpu=750 -mstring -mmultiple -noixemul -fstrict-aliasing -I../include/
LDFLAGS = -noixemul

CWARN = -Wall\
	-Wno-missing-braces\
	-Wno-parentheses\
        -Wno-switch\
        -Wstrict-prototypes\
	-Werror-implicit-function-declaration\
        -Wpointer-arith

OBJS = libinit.o init.o utils.o loc.o obsolete.o CManager.o

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -nostartfiles -o $@ $(OBJS)
	ppc-morphos-strip $@

install:
	@copy $(TARGET) $(DEST)
	@delete $(TARGET)
	@avail flush

libinit.o:    libinit.c lib.h CManager.library_rev.h base.h lib_protos.h
init.o:       init.c lib.h base.h lib_protos.h
loc.o:        loc.c lib.h base.h lib_protos.h
utils.o:      utils.c lib.h base.h lib_protos.h
obsolete.o:   obsolete.c lib.h base.h lib_protos.h
CManager.o:   CManager.c lib.h base.h lib_protos.h
