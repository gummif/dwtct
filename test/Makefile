
include ../Makefile.inc

CFLAGS += -Wall -Wextra -I$(INCDIR) 
LDFLAGS += -L$(LIBDIR) -l$(NAME)

TARGET = test$(NAME)
SRC = $(TARGET).c
OBJ = $(SRC:.c=.o)


.PHONE: all clean run

all: $(TARGET)

$(TARGET): $(TARGET).o
	$(CC) $(LDFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ) $(TARGET)

run:
	@./$(TARGET)
