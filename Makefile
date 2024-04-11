OBJS	= src/main.o src/types.o src/vec3.o src/debug.o src/stack.o
SOURCE	= src/main.c src/types.c src/vec3.c src/debug.c src/stack.c
HEADER	= src/types.h src/vec3.h src/debug.h src/stack.h
OUT	= raytracer1d
LFLAGS	 = -lm -Wall

all: raytracer1d

raytracer1d: $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c $(HEADER)
	$(CC) -c -o $@ $< $(LFLAGS)

clean:
	rm -f $(OBJS) $(OUT)
