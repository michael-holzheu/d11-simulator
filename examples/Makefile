include ../common.mak

ALL_CPPFLAGS += -g

TARGETS = example

all: $(TARGETS)

libs =	\
	$(rootdir)/libd11/libd11.a \
	$(rootdir)/libutil/libutil.a

example: example.o $(libs)

install: all

clean:
	rm -f *.o *~ $(TARGETS)

.PHONY: all install clean
