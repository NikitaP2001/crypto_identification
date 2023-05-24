LIBDIR = $(realpath lib)

CC = @gcc
LD = @gcc
AR = @x86_64-w64-mingw32-ar
RM = @-rm -rf


CFLAGS = -c -g -std=c11 -Wall -Wextra -Wundef -pedantic -I $(LIBDIR)
LDFLAGS = -static -fstack-protector -L $(LIBDIR)
LDLIBS = -lcheck

export

all:
	@$(MAKE) -C lib --no-print-directory
	@$(MAKE) -C schnorr --no-print-directory	
	@$(MAKE) -C fa3x --no-print-directory	
	@$(MAKE) -C ffs --no-print-directory	
	
test: all
	@$(MAKE) -C lib test --no-print-directory
	@$(MAKE) -C schnorr test --no-print-directory
	@$(MAKE) -C fa3x test --no-print-directory
	@$(MAKE) -C ffs test --no-print-directory
	
runtest:
	@$(MAKE) -C lib runtest --no-print-directory
	@$(MAKE) -C schnorr runtest --no-print-directory
	@$(MAKE) -C fa3x runtest --no-print-directory
	@$(MAKE) -C ffs runtest --no-print-directory
	
.PHONY: clean
clean:
	@$(MAKE) -C lib clean --no-print-directory
	@$(MAKE) -C schnorr clean --no-print-directory	
	@$(MAKE) -C fa3x clean --no-print-directory	
	@$(MAKE) -C ffs clean --no-print-directory	