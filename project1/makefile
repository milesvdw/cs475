#Miles Van de Wetering
#vandewmi@engr.oregonstate.edu
#CS475
#Project 1

CC=g++
CFLAGS = -fopenmp
PROGS=reimann_sum

all: $(PROGS)

reimann_sum: main.cpp                            
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(PROGS) *.o *~ \#*
	
