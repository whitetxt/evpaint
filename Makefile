filename = paint

CC = gcc
CFLAGS = -g `pkg-config --cflags --libs sdl2` -lSDL2_image -lSDL2_ttf -lcomdlg32 -lole32 -mconsole

files = ${wildcard *.c}
objFiles = ${files:.c=.o}
headers = ${wildcard *.h}

all:
	${CC} -o ${filename} ${files} ${CFLAGS} 

clean:
	-rm -rf ${filename}
	-rm -rf ${objFiles}