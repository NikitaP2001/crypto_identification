SRC = benchmark.c schnorr.c
TSRC = schnorr.c test.c
OBJ = $(SRC:%.c=%.o)
TOBJ = $(TSRC:%.c=%.o)

CFLAGS += -msse -msse2 -msse3 -msse4.1 -msha
LDLIBS += -lcheck -lgmp

%.o: %.c
	@echo CC $<
	$(CC) $(CFLAGS) -o $@ $<
	
all: bench.exe

bench: $(OBJ)
	@echo LD $<
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

test: $(TOBJ)
	@echo LD $<
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

.PHONY: clean
clean:
	@$(RM) bench.exe
	@$(RM) test.exe
	@$(RM) $(OBJ)
	@$(RM) $(TOBJ)