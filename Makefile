#
# Makefile
#

CFLAGS		:= -Wall -Wextra -fdiagnostics-color=auto -std=c89 -lm
LDLIBS		:= -lm -lglfw -lGL -lGLU -lalut -lpng

b3d-o		= src/main.o
b3d-i		= -I src/include

all: b3d run

PHONY += clean
clean:
	@echo -e "\033[1m> Removing binaries...\033[0m"
	@find src -type f -name '*.o' -exec rm {} \;
	@rm -f bin/b3d

%.o: %.c
	@echo -e "\033[1;37m> Compiling \033[0;32m$<\033[1m...\033[0m"
	@$(CC) $(CFLAGS) -c $< $(b3d-i) -o $@

PHONY += b3d
b3d: bin/b3d
bin/b3d: $(b3d-o)
	@echo -e "\033[1m> Linking \033[0;32m$@\033[1m...\033[0m"
	@mkdir -p bin/
	@$(CC) $(LDLIBS) -o bin/b3d $(b3d-o)

run: bin/b3d
	@echo -e "\033[1m> Running b3d...\033[0m"
	@./bin/b3d

.PHONY: $(PHONY)
