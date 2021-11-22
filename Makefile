GL_PKGS=sdl2

CC=Clang
CFLAGS=-Wall -Wextra

all: rule110 visualization

rule110: rule110.c
	$(CC) $(CFLAGS) -o rule110 rule110.c

visualization: visualization.c
	$(CC) $(CFLAGS)  `pkg-config --cflags $(GL_PKGS)` -o visualization visualization.c `pkg-config --libs $(GL_PKGS)` -lm
