CC = gcc
OBJS = main.o ArrayList.o Button.o ChessGame.o ChessParser.o ChessWindow.o MainAux.o Minimax.o \
	SDL_main.o SDL_MainAux.o Widget.o
 
EXEC = chessprog
CC_COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
main.o: main.c MainAux.h SDL_main.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c

ArrayList.o: ArrayList.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
	
Button.o: Button.h SDL_MainAux.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
	
ChessGame.o: ChessGame.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
	
ChessParser.o: ChessParser.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
	
ChessWindow.o: ChessWindow.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c	

MainAux.o: MainAux.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
	
Minimax.o: Minimax.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
	
SDL_main.o: SDL_MainAux.h MainAux.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c		
	
SDL_MainAux.o: Button.h ChessWindow.h SDL_MainAux.h MainAux.h ChessGame.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
	
Widget.o: Widget.h
	$(CC) $(CC_COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c	
	
clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
