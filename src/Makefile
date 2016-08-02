CC= clang
CFLAGS=-I../include
CFLAGSDEBUG=-g -O3
LIBS = -lSDL2 -lSDL2_image -lSDL2_ttf
DEPS = $(addprefix,../include/,$(wildcard *.h))
SOURCE = $(addprefix,../src/,$(wildcard *.c))
OBJS = $(addprefix ../build/release/,$(patsubst %.c,%.o,$(wildcard *.c)))
DEBUG_OBJS = $(addprefix ../build/debug/,$(patsubst %.c,%.o,$(wildcard *.c)))

release: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	mv release ../build/release/

../build/release/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

debug: $(DEBUG_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
	mv debug ../build/debug/
../build/debug/%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS) $(CFLAGSDEBUG)
clean: 
	 rm -rf ../build/debug/*
	 rm -rf ../build/release/*
