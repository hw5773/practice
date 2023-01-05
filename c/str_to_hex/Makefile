CC=gcc
RM=rm

BIN=str_to_hex
SRC=str_to_hex.c
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

