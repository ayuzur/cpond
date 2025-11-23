obj = main.o draw.o fish.o sleep.o llist.o utils.o braille.o
# you may need to change -lncursesw to -lncurses if you are on MacOS
link=-lncursesw -lm
ncursesw_macros=-D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600
#$$(ncursesw6-config --cflags)
eflags=-Wpedantic -Wall -Wextra 
#-fsanitize=float-divide-by-zero 
#-g3
#-Wconversion

cpond: $(obj) 
	gcc $(obj) $(link) -o cpond

main.o: draw.h fish.h sleep.h llist.h utils.h braille.h
	gcc -c main.c $(eflags)

draw.o: draw.c draw.h
	gcc -c draw.c $(eflags)$(ncursesw_macros)

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
