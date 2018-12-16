CC=g++
CFLAGS=-I.
TESTFLAGS=-lgtest -lpthread
DEPS = MyStackContainer.h
OBJ = MyStackContainer.o app.o 
TESTOBJ = MyStackContainer.o test.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: app test

app: $(OBJ)
	$(CC) -o $@ $^

test: $(TESTOBJ)
	$(CC) -o $@ $^ $(TESTFLAGS)

clean:
	rm -f app test $(OBJ) $(TESTOBJ)
