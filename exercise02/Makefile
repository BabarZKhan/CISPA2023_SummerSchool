all: kaslr_break

kaslr_break: kaslr_break.c
	gcc kaslr_break.c -Os -Wall -o kaslr_break

solution: kaslr_break_solution.c
	gcc kaslr_break_solution.c -Os -static -Wall -o solution

clean:
	rm -f kaslr_break solution
