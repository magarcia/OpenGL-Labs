# Get OS Type
UNAME := $(shell uname)

# Makefile
CC      = g++
CFLAGS 	= -Wall -O3 -c
ifeq ($(UNAME), Linux)
LFLAGS	= -lglut -lGL -lGLU -lX11 -lXmu -lm
endif
ifeq ($(UNAME), Darwin)
LFLAGS = -framework Carbon -framework OpenGL -framework GLUT
endif

BUILDIR	= build
EXEC	= $(BUILDIR)/idi
OBJS	= main.o point.o model.o

# Pretty print
V = @
Q = $(V:1=)
Q_CC = $(Q:@=@echo    '     CPP      '$@;)
Q_LD = $(Q:@=@echo    '     LD       '$@;)


.cpp.o:
	$(Q_CC) $(CC) $(CFLAGS) -c -o $@ $<

$(EXEC): $(OBJS)
	@mkdir -p $(BUILDIR)
	$(Q_LD) $(CC) $(LFLAGS) -o $@ $(OBJS)

run: $(EXEC)
	@$(EXEC)

clean:
	@(rm -rf $(OBJS) $(EXEC))

world: clean $(EXEC)

.PHONY: clean world
