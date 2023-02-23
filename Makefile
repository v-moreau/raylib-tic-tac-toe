NAME=tic-tac-toe
CC=clang
CFLAGS=-framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL libraylib.a

first: $(NAME).c
	$(CC) $(CFLAGS) $(NAME).c -o $(NAME)

clean:
	rm -f $(NAME) *.o

.PHONY: run
run: $(NAME)
	./$(NAME)