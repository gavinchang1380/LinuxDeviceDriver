
FILES = nbtest load50 mapcmp polltest mapper setlevel setconsole inp outp \
	datasize dataalign netifdebug asynctest inb inw inl outb outw outl

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
INCLUDEDIR = $(KERNELDIR)/include
CFLAGS = -O2 -fomit-frame-pointer -Wall -I$(INCLUDEDIR)

all: $(FILES)

inb: inp
	ln inp inb -s

inw: inp
	ln inp inw -s

inl: inp
	ln inp inl -s

outb: outp
	ln outp outb -s

outw: outp
	ln outp outw -s

outl: outp
	ln outp outl -s

clean:
	rm -f $(FILES) *~ core

