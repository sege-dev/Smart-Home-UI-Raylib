
RAYLIB_PATH = raylib
CFLAGS = -I$(RAYLIB_PATH)/include
LDFLAGS = -L$(RAYLIB_PATH)/lib -lraylib -lopengl32 -lgdi32 -lwinmm

all:
	gcc gui_test.c -o gui_test.exe $(CFLAGS) $(LDFLAGS)