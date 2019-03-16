CC = gcc
LD = gcc
#CFLAGS = -Wall -Ofast -fopt-info-vec
#CFLAGS = -Wall -O3 -march=native -fopt-info-vec
CFLAGS = -Wall -Ofast -march=haswell -ffast-math -fopt-info-vec -g
LIBS = -L/opt/X11/lib -lX11 -lm
RM = /bin/rm -f

OBJ1 = graphics.o V1.o
OBJ2 = graphics.o V2.o
OBJ3 = graphics.o V3.o

EXEC = galsim

V1: $(OBJ1)
	$(LD) $(LDFLAGS) $(OBJ1) $(LIBS) -o $(EXEC)

V2: $(OBJ2)
	$(LD) $(LDFLAGS) $(OBJ2) $(LIBS) -o $(EXEC)

V3: $(OBJ3)
	$(LD) $(LDFLAGS) $(OBJ3) $(LIBS) -o $(EXEC)

graphics.o: graphics.c graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c graphics.c

V1.o: V1.c graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c V1.c

V2.o: V2.c graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c V2.c

V3.o: V3.c graphics.h
	$(CC) $(CFLAGS) $(INCLUDES) -c V3.c

clean:
	$(RM) $(EXEC) $(OBJ1) $(OBJ2) $(OBJ3)  
