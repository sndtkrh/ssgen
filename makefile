
TARGET = ssgen
SRCDIR = ./src
OBJDIR = ./build
SOURCES = $(shell ls $(SRCDIR)/*.cpp)
OBJS = $(subst $(SRCDIR),$(OBJDIR), $(SOURCES:.cpp=.o))
CC = g++
CFLAGS = -std=c++11 -Wall -O2

.SUFFIXES: .o .cpp

.PHONY: all clean

all: $(TARGET)
clean:
	rm -f $(OBJS) $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp 
	@if [ ! -d $(OBJDIR) ]; then echo "mkdir -p $(OBJDIR)"; mkdir -p $(OBJDIR); fi
	$(CC) $(CFLAGS) -o $@ -c $<

main.o: star.hpp
stars.o: star.hpp

