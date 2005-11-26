
CC 	= ppc-morphos-gcc
CDEFS   =
CFLAGS  = $(CDEFS) $(CWARN) -O2 -fomit-frame-pointer -mcpu=750 -mstring -mmultiple -noixemul -fstrict-aliasing -I../include/
LDFLAGS = -noixemul
CWARN   = -Wall\
	  -Wno-missing-braces\
	  -Wno-parentheses\
          -Wno-switch\
          -Wstrict-prototypes\
	  -Werror-implicit-function-declaration\
          -Wpointer-arith

OBJS =  CMURLAdd.o

CMURLAdd: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)
	ppc-morphos-strip --strip-unneeded --remove-section=.comment $@

CMURLAdd.o: CMURLAdd.c CMURLAdd_rev.h

