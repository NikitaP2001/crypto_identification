SRC = benchmark.c schnorr.c sha256.c
TSRC = schnorr.c test.c sha256.c
OBJ = $(SRC:%.c=%.o)
TOBJ = $(TSRC:%.c=%.o)

CC = @-gcc
LD = @-gcc
RM = @-rm -rf

CFLAGS = -c -g -std=c99 -Wall -Wextra -Wundef -pedantic -mrdseed \
-msse -msse2 -msse3 -msse4.1 -msha
LDLIBS = -lcheck -lgmp

%.o: %.c
	@echo CC $<
	$(CC) $(CFLAGS) -o $@ $<

all: bench

bench: $(OBJ)
	$(LD) -o $@ $^ $(LDLIBS)

test: $(TOBJ)
	$(LD) -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:
	@$(RM) $(OBJ)
	@$(RM) $(TOBJ)