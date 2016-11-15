#
# Makefile
#

CFLAGS		:= -Wall -Wextra -fdiagnostics-color=auto -std=c89
LDLIBS		:= -lm -lglfw -lGL -lGLU

bge-o		= src/main.o
bge-i		= -I src/include

all: bge run

PHONY += clean
clean:
	@echo -e "\033[1m> Removing binaries...\033[0m"
	@find src -type f -name '*.o' -exec rm {} \;
	@rm -f bin/bge

%.o: %.c
	@echo -e "\033[1;37m> Compiling \033[0;32m$<\033[1m...\033[0m"
	@$(CC) $(CFLAGS) -c $< $(bge-i) -o $@

PHONY += bge
bge: bin/bge
bin/bge: $(bge-o)
	@echo -e "\033[1m> Linking \033[0;32m$@\033[1m...\033[0m"
	@mkdir -p bin/
	@$(CC) $(LDLIBS) -o bin/bge $(bge-o)

run: bin/bge
	@echo -e "\033[1m> Running bge...\033[0m"
	@./bin/bge

.PHONY: $(PHONY)
