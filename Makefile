LIBDIR = $(realpath lib)

CC = @-gcc
LD = @-gcc
AR = @-x86_64-w64-mingw32-ar
RM = @-rm -rf


CFLAGS = -c -g -std=c99 -Wall -Wextra -Wundef -pedantic -I $(LIBDIR)
LDFLAGS = -static -fstack-protector -L $(LIBDIR)
LDLIBS = -lgmptools

export

all:
	@$(MAKE) -C lib --no-print-directory
	@$(MAKE) -C schnorr --no-print-directory	
	@$(MAKE) -C fa3x --no-print-directory	
	
test:
	@$(MAKE) -C lib --no-print-directory
	@$(MAKE) -C schnorr test --no-print-directory
	@$(MAKE) -C fa3x test --no-print-directory
	
.PHONY: clean
clean:
	@$(MAKE) -C lib clean --no-print-directory
	@$(MAKE) -C schnorr clean --no-print-directory	
	@$(MAKE) -C fa3x clean --no-print-directory	