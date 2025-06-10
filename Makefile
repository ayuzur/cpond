
obj = draw.o

cpond: main.c $(obj)
	gcc main.c $(obj) -lncurses -lm -o cpond

draw.o: draw.c draw.h
	gcc -c draw.c
run:
	./cpond

clean: 
	rm cpond $(obj)
