TARGET = libgmptools.a

SRC = gmptools.c
OBJ = $(SRC:%.c=%.o)
TOBJ = $(TSRC:%.c=%.o)

CFLAGS += -mrdseed
LDLIBS = -lgmp

%.o: %.c
	@echo CC $<
	$(CC) $(CFLAGS) -o $@ $<

all: $(TARGET)
	
$(TARGET): $(OBJ)
	@echo AR $<
	$(AR) rsc -o $@ $^ $(LDLIBS)

.PHONY: clean
clean:	
	@$(RM) $(TARGET)
	@$(RM) $(OBJ)