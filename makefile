
# names
DST_DIR = ./build/
DST_NAME = uyghur.exe
SRC = ./main.c
DST = $(DST_DIR)$(DST_NAME)
SCRIPT = ./examples/test.ug

# raylib
FOLDER_RAYLIB = ./external/raylib-4.0.0_win64_mingw-w64
LFLAGS_RAYLIB = -I $(FOLDER_RAYLIB)/include -L $(FOLDER_RAYLIB)/lib
CFLAGS_RAYLIB = -lraylib -lopengl32 -lgdi32 -lwinmm

# raygui
FLAGS_RAYGUI = -I ./external/raygui-master

# console
LFLAGS_EXTRA = $(FLAGS_RAYGUI)
CFLAGS_CONSOLE = # -O2 -mwindows

# flags
LFLAGS = $(LFLAGS_RAYLIB) $(LFLAGS_EXTRA)
CFLAGS = $(CFLAGS_RAYLIB) $(CFLAGS_CONSOLE)

# compiler
CC = gcc

# commands
run: $(SRC)
	@mkdir -p $(DST_DIR)
	@rm -rf $(DST_DIR)*
	@cp $(SCRIPT) $(DST_DIR)script.ug
	@echo "# uyghur script!" >> $(DST_DIR)script.ug
	@xxd -i $(DST_DIR)script.ug > ./build/script.h
	@$(CC) $(SRC) $(LFLAGS) $(CFLAGS) -o $(DST)
	@cd $(DST_DIR) && ./$(DST_NAME) ../$(SCRIPT)
