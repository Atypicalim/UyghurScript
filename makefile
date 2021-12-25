
# names
DST_DIR = ./release/
DST_NAME = uyghur.exe
SRC = ./main.c
DST = $(DST_DIR)$(DST_NAME)
SCRIPT = ../examples/test.ug

# raylib
FOLDER_RAYLIB = ./external/raylib-4.0.0_win64_mingw-w64
LFLAGS_RAYLIB = -I $(FOLDER_RAYLIB)/include -L $(FOLDER_RAYLIB)/lib
CFLAGS_RAYLIB = -lraylib -lopengl32 -lgdi32 -lwinmm

# console
LFLAGS_EXTRA = # -O2 -mwindows
CFLAGS_EXTRA = # -O2 -mwindows

# flags
LFLAGS = $(LFLAGS_RAYLIB) $(LFLAGS_EXTRA)
CFLAGS = $(CFLAGS_RAYLIB) $(CFLAGS_CONSOLE)

# compiler
CC = gcc

# commands
run: $(SRC)
	@rm -rf $(DST_DIR)*
	@$(CC) $(SRC) $(LFLAGS) $(CFLAGS) -o $(DST)
	@cd $(DST_DIR) && ./$(DST_NAME) $(SCRIPT)
