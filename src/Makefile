
include ../Makefile.inc

threaded ?= 1
ifeq ($(threaded), 0)
	#
else
	CFLAGS += -fopenmp
	LDFLAGS += -lgomp
endif


CFLAGS += -Wall -Wextra -fPIC -DDWTCT_EXPORT -funroll-loops -funsafe-loop-optimizations
LDFLAGS += -shared -fPIC -lc $(SONAME)

TARGET = $(LIBTARGET)
LIBHEADERS = dwtct.h dwtctconf.h
SRC = dwtct.cpp
OBJ = $(SRC:.cpp=.o)


.PHONE: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ) $(TARGET)

# deps
dwtct.o: dwtct.cpp $(LIBHEADERS)
