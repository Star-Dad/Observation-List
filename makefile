OBJECTS = main.o julian_day.o read_NGC.o

all: $(OBJECTS)
	/usr/bin/gcc $(OBJECTS) -lm  -o observation


main.o::	main.c
julian_day.o: 	julian_day.c
read_NGC.o:	read_NGC.c
