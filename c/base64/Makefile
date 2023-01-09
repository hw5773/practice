CC=gcc
SRCS=base64_test.c base64.c bit_op.c
OBJS=$(SRCS:.c=.o)
BIN=base64_test

CFLAGS=-DDEBUG_LEVEL=2
ifeq ($(TEST), 1)
	CFLAGS+=-DTEST
endif

all: base64_test

base64_test: $(OBJS)
	$(CC) -o $@ $(OBJS) $(COMMON_LDFLAGS)
	@echo "LD <= $<"

%.o: %.c
	$(CC) -c $< $(CFLAGS) 
	@echo "CC <= $<"

clean:
	$(RM) $(OBJS) $(BIN)
