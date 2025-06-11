
obj = draw.o fish.o sleep.o 

cpond: main.c $(obj)
	gcc main.c $(obj) -lncurses -lm -o cpond

draw.o: draw.c draw.h
	gcc -c draw.c

fish.o: fish.c fish.h
	gcc -c fish.c

sleep.o: sleep.c sleep.h
	gcc -c sleep.c

run:
	./cpond
clean: 
	rm cpond $(obj)
