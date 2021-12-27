CC=gcc 
CFLAGS= -Wall -fopenmp 
EXE=launcher  
all: launcher.o     
	rm -f $(EXE)     
	$(CC) $(CFLAGS) launcher.o  -o $(EXE)  

exe: launcher.o     
	clear     
	./$(EXE)  

launcher.o: launcher.c launcher.h     
	$(CC) $(CFLAGS) -c launcher.c  
	
clean:     
	rm -f *.o     
	rm -f $(EXE)