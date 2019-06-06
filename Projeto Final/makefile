# Uso:
# make        # compila o aplicativo
# make clean  # deleta o binário

.PHONY: install clean

CC = gcc
CFLAGS = -g
LIBS = -lpthread -lrt

APPNAME = a.out # a140016686chat
CODE = main.c

install:
	@echo "Compilando aplicativo de chat ..."
	@$(CC) $(CFLAGS) $(CODE) -o $(APPNAME) $(LIBS)

clean:
	@echo "Deletando binário ..."
	@rm -f $(APPNAME)
