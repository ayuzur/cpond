
obj = draw.o fish.o sleep.o llist.o utils.o braille.o
flags=-lncursesw -lm $$(ncursesw6-config --cflags)
eflags=-Wpedantic -Wall -Wextra -fsanitize=float-divide-by-zero -g3
#-Wconversion

cpond: main.c $(obj) 
	gcc main.c $(obj) $(flags) -o cpond $(eflags)

draw.o: draw.c draw.h
	gcc -c draw.c $(flags) $(eflags)

fish.o: fish.c fish.h
	gcc -c fish.c $(eflags)

sleep.o: sleep.c sleep.h
	gcc -c sleep.c $(eflags)

llist.o: llist.c llist.h
	gcc -c llist.c $(eflags)

utils.o: utils.c utils.h
	gcc -c utils.c $(eflags)

braille.o: braille.c braille.h
	gcc -c braille.c $(eflags)

run:
	./cpond
clean: 
	rm cpond $(obj)
