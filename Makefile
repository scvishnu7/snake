all: snake

snake: snake.c
	gcc -o snake snake.c -lglut -lGL -lGLU -lm
clean:
	rm snake
