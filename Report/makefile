.default: build

.PNONY: build clean
build: main.c app.c
	gcc -fopenmp main.c app.c -o out -lm
clean:
	rm out
