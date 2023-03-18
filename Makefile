CC = g++
CFLAGS = -Wall -g
HEADERS = -I include
ARGS = -std=c++11 -lstdc++

TARGET = test.out

SRC = src/Json.cpp src/Parser.cpp src/test.cpp
OBJECT = obj/Json.o obj/Parser.o obj/test.o

all : $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) $(HEADERS) $(ARGS) $(OBJECT) -o $@

obj/%.o : src/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(HEADERS) $(ARGS) -c $< -o $@

clean:
	rm -f $(TARGET)
	rm -f obj/*.o
