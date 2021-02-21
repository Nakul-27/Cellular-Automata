GL3_PKGS=glfw3 glew

CC=Clang
CFLAGS=-Wall -Wextra
GFLAGS=-Wno-deprecated-declarations

all: rule110 visualization

rule110: rule110.c
	$(CC) $(CFLAGS) -o rule110 rule110.c

visualization: visualization.c
	$(CC) $(CFLAGS) $(GFLAGS) `pkg-config --cflags $(GL3_PKGS)` -o visualization visualization.c `pkg-config --libs $(GL3_PKGS)`
