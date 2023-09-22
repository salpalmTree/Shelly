VPATH = ./src/:./inc/

shelly: main.o shelly_func.o
	gcc main.o shelly_func.o -o shelly

main.o: main.c
	gcc -c $^ -o $@

shelly_func.o: shelly_func.c shelly_func.h
	gcc -c $< -o $@

clean:
	rm *.o shelly


