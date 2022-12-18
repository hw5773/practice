CC=gcc
RM=rm

BIN=sip_controller
SRC=sip_controller.c sip_controller_local.c
OBJ=$(SRC:.c=.o)
CFLAGS=
LDFLAGS=

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

.c.o: 
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	$(RM) $(BIN) $(OBJ)

