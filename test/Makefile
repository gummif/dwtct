
include ../Makefile.inc

CFLAGS += -Wall -Wextra -I$(INCDIR) 
LDFLAGS += -L$(LIBDIR) -l$(NAME)

TARGET = test$(NAME)
SRC = $(TARGET).c
SRCXX = dwtct_base.cpp
OBJ = $(SRC:.c=.o) $(SRCXX:.cpp=.o)

.PHONE: all clean run

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ) $(TARGET)

run:
	@./$(TARGET)
