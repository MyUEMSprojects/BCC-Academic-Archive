CXX = gcc

CXXFLAGS = -Wall -Wextra -pedantic -pthread

%: %.c
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

clean:
	rm -f $(basename $(wildcard *.c))
