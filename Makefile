CC=g++
FILES=tema3.cpp
TARGET=navigator
build:$(FILES)
	$(CC) -o $(TARGET) $(FILES)
clean:
	rm -f $(TARGET)