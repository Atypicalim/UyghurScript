
# names
DST_DIR = ./release/
DST_NAME = uyghur.exe
SRC = ./main.c
DST = $(DST_DIR)$(DST_NAME)
SCRIPT = ../examples/test.ug

# flags
LFLAGS = # -I some/include/dir -L some/lib/dir

# compile flags
CFLAGS = # -O2 -mwindows

# compiler
CC = gcc

# commands
run: $(SRC)
	@rm -rf $(DST_DIR)*
	@$(CC) $(SRC) $(LFLAGS) $(CFLAGS) -o $(DST)
	@cd $(DST_DIR) && ./$(DST_NAME) $(SCRIPT)
