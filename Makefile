CXX = D:/CodeBlocks/share/CodeBlocks/compilers/mingw64/bin/g++.exe
CXXFLAGS = -std=c++20 -O3 -Wall -Wextra -pedantic

SRCS = input.cpp Upload.cpp Usluga.cpp KatalogUslug.cpp Pracownik.cpp Zgloszenie.cpp \
       AlgorytmA.cpp AlgorytmB.cpp AlgorytmC.cpp AlgorytmAging.cpp \
       Statystyki.cpp SilnikSymulacji.cpp Output.cpp GeneratorZgloszen.cpp main.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = symulator.exe

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q $(OBJS) $(TARGET)
