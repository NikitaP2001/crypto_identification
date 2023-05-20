TARGET = libgmptools.a

SRC = gmptools.c sha256.c
TSRC = gmptools.c sha256.c test.c
OBJ = $(SRC:%.c=%.o)
TOBJ = $(TSRC:%.c=%.o)

CFLAGS += -mrdseed -msse -msse2 -msse3 -msse4.1 -msha
LDLIBS += -lgmp

%.o: %.c
	@echo CC $<
	$(CC) $(CFLAGS) -o $@ $<

all: $(TARGET)
	
$(TARGET): $(OBJ)
	@echo AR $<
	$(AR) rsc -o $@ $^

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
	@$(RM) test.exe
	@$(RM) $(TARGET)
	@$(RM) $(OBJ)
	@$(RM) $(TOBJ)