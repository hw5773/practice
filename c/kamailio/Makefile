CC=gcc
RM=rm

BIN=test_sip_controller
SRC=test_sip_controller.c sip_controller.c
OBJ=$(SRC:.c=.o)
CFLAGS=-DDEBUG_LEVEL=2
LDFLAGS=

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) $(BIN) $(OBJ)

