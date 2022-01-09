CC=gcc
CFLAGS= -Wall -fopenmp
EXE=bahraoui_guendouz_PUISS
MAIN=BAHRAOUIMarwane-GUENDOUZReda_codeSourcePUISS.c

all: $(MAIN)
	@if [ -e $(EXE) ]; then rm ./$(EXE); fi
	$(CC) $(CFLAGS) $(MAIN) -o $(EXE)

exe: $(EXE)
	./$(EXE)  

clean:
	rm -f *.o
	rm -f $(EXE)