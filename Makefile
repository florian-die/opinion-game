CC = gcc
CFLAGS = -Wall -pedantic
LIBS = -lSDL2
SRC = dynamics.c game.c main.c objectives.c sdl_utils.c state_list.c trajectories.c
EXE = launcher.exe

$(EXE): 
	$(CC) $(CFLAGS) $(SRC) $(LIBS) -o $(EXE)

# Clean Up Objects, Exectuables, Dumps out of source directory
clean:
	$(RM) $(OBJ) $(EXE) a.out