CC=gcc 
CFLAGS= -Wall -fopenmp 
EXE=para  
all: main.o     
	rm -f $(EXE)     
	$(CC) $(CFLAGS) main.o  -o $(EXE)  

exe: main.o     
	clear     
	./$(EXE)  

main.o: main.c main.h     
	$(CC) $(CFLAGS) -c main.c  
	
clean:     
	rm -f *.o     
	rm -f $(EXE)