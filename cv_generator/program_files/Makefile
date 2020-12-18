CC := gcc
CFLAGS := -Wall -O
HEADERS := read.h filter.h format.h
OBJ := generate_cv
OBJFILES := levenshtein.o read.o filter.o format.o main.o

all: $(OBJ)
$(OBJ): $(OBJFILES)
	$(CC) $(CFLAGS) $(OBJFILES) -o $(OBJ) 
read.o: read.c $(HEADERS)
	$(CC) -c $(CFLAGS) read.c
levenshtein.o: levenshtein.c
	$(CC) -c $(CFLAGS) levenshtein.c
filter.o: filter.c $(HEADERS)
	$(CC) -c $(CFLAGS) filter.c
format.o: format.c $(HEADERS)
	$(CC) -c $(CFLAGS) format.c
main.o: main.c $(HEADERS)
	$(CC) -c $(CFLAGS) main.c
run: $(OBJ)
	./generate_cv
clean:
	rm -rf generate_cv
	rm -rf *.exe
	rm -rf *.o
