power: power.o test-power.o
	gcc -o power power.o test-power.o -lm
power.o: power.c
	gcc -c -pedantic -std=gnu99 -Wall -Werror power.c
test-power.o: test-power.c
	gcc -c -pedantic -std=gnu99 -Wall -Werror test-power.c
