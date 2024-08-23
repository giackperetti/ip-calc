CC = gcc
CFLAGS = -Wall -O2
LDFLAGS =

SRC = src/ip-calc.c
TARGET = bin/ip-calc

.PHONY: run build clean

run: build
	./$(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

build: $(TARGET)
	@echo -e "\033[1mBuild ended succesfully!\033[0m"

default: build

clean:
	rm -f $(TARGET)
