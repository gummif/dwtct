
include ../Makefile.inc


CFLAGS += -std=c99 -Wall -Wextra -fPIC -DDWTCT_EXPORT
LDFLAGS += -shared -fPIC -lc $(SONAME)

TARGET = $(LIBTARGET)
LIBHEADERS = dwtct.h dwtctconf.h
SRC = dwtct.c
OBJ = $(SRC:.c=.o)


.PHONE: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ) $(TARGET)

# deps
dwtct.o: dwtct.c $(LIBHEADERS)
