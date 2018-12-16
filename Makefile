CC=g++
CFLAGS=-I.
TESTFLAGS=-lgtest -lpthread
DEPS = MultiStack.h
OBJ = MultiStack.o app.o 
TESTOBJ = MultiStack.o test.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: app test

app: $(OBJ)
	$(CC) -o $@ $^

test: $(TESTOBJ)
	$(CC) -o $@ $^ $(TESTFLAGS)

clean:
	rm -f app test $(OBJ) $(TESTOBJ)
