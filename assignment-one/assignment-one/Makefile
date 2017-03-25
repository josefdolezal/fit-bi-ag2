CXX=g++
CXXFLAGS=-Wall -pedantic -Wextra -std=c++11

compile: main.cpp
	$(CXX) $(CXXFLAGS) -o main.o ./main.cpp

tests: compile
	./tests.sh
