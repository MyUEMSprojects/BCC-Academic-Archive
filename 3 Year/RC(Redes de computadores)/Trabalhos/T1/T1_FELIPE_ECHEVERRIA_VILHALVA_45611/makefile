# Define o compilador C++
CXX = g++

CXXFLAGS = -Wall -std=c++17 -O2

LIBS = -pthread

all: cliente servidor

cliente: cliente.cpp
	$(CXX) $(CXXFLAGS) -o cliente cliente.cpp $(LIBS)

servidor: servidor.cpp
	$(CXX) $(CXXFLAGS) -o servidor servidor.cpp $(LIBS)

clean:
	rm -f cliente servidor

.PHONY: all clean
