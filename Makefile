CXX := g++
CFLAGS := -O3 -std=c++17 -march=native
TARGET = squamigera

all: $(TARGET)

$(TARGET): main.o
	g++ $(CFLAGS) -o $(TARGET) main.o -lgdi32

main.o: main.cpp
	g++ -c $(CFLAGS) -o main.o main.cpp

clean:
	rm -f main.o $(TARGET)

.PHONY: clean
