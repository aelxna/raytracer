OBJS	= src/main.o src/types.o src/vec3.o src/debug.o
SOURCE	= src/main.c src/types.c src/vec3.c src/debug.c
HEADER	= src/types.h src/vec3.h src/debug.h
OUT	= raytracer1c
LFLAGS	 = -lm -Wall

all: raytracer1c

raytracer1c: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(LFLAGS)

clean:
	rm -f $(OBJS) $(OUT)
