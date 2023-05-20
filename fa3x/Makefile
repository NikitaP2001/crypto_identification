SRC = benchmark.c fa3x.c
TSRC = fa3x.c test.c
OBJ = $(SRC:%.c=%.o)
TOBJ = $(TSRC:%.c=%.o)

LDLIBS += -lgmptools -lgmp

%.o: %.c
	@echo CC $<
	$(CC) $(CFLAGS) -o $@ $<
	
all: bench

bench: $(OBJ)
	@echo LD $<
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 

test: $(TOBJ)
	@echo LD $<
	$(LD) -o $@ $^ $(LDLIBS) $(LDFLAGS) 
	
.PHONY: memtest
memtest:
	$(DRMEM) $(LOG_DIR) -- test.exe
	
.PHONY: runtest
runtest:
	@-./test.exe

.PHONY: clean
clean:
	@$(RM) bench.exe
	@$(RM) test.exe
	@$(RM) $(OBJ)
	@$(RM) $(TOBJ)