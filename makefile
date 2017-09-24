
CC = cc

CFLAGS = -c -Wall

EXECUTABLE_NAME = ls
SOURCES = my_ls.c

OBJECTS = $(SOURCES:.c=.o)

all: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME): $(OBJECTS)
	$(CC) -o $(EXECUTABLE_NAME) $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) *.o *~ $(EXECUTABLE_NAME)
