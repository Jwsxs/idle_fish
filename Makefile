
SRC=src/main.c
INCL=include/
LIBS=lib/

EXEC = chillq.exe

all: $(EXEC)

$(EXEC):
	echo -e '\0033\0143'
	gcc $(SRC) -o $(EXEC) -I$(INCL) -L$(LIBS)  -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

run:
	echo "Running. . ."
	./$(EXEC)

clean:
	echo "Cleaning. . ."
	rm -f $(EXEC)

# .SILENT:
.PHONY: all