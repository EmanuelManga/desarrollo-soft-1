CC      = gcc
CFLAGS  = -ansi -pedantic-errors -Wall -Wextra
LDFLAGS =

# Compilar un .c: make archivo   (sin extensión)
# Ejemplo: make clase_01_09
%:
	$(CC) $(CFLAGS) $(LDFLAGS) $@.c -o $@

.PHONY: clean
clean:
	find . -maxdepth 2 -type f -perm /111 ! -name '*.sh' -exec rm -f {} +
