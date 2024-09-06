main: main.c
	clang main.c -o main -Wall -Wextra -pedantic -Werror -lraylib -L./raylib/src/ -I./raylib/src/ -lm -L/opt/homebrew/lib/ -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit