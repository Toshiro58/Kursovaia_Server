.PHONY: all clean format test doxy

CXX=g++
CXXFLAGS=-O2 -Wall -DNDEBUG -std=c++17
LDFLAGS= -lcryptopp -lboost_program_options -lUnitTest++
PROJECT = Server
SOURCES := $(filter-out test.cpp main.cpp, $(wildcard *.cpp))
HEADERS := $(wildcard *.h)
OBJECTS := $(SOURCES:%.cpp=%.o)

all : $(PROJECT)

$(PROJECT) : $(OBJECTS) main.o
	$(CXX) $^ $(LDFLAGS) -o $@
	rm -f *.o *.orig

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJECTS) : $(HEADERS)

format:
	astyle *.cpp *.h

doxy:
	doxygen

clean:
	rm -f $(PROJECT) test_runner *.o *.orig

test: test_runner
	./test_runner

test_runner: test.o $(OBJECTS)
	$(CXX) test.o $(OBJECTS) $(LDFLAGS) -o test_runner

test.o: test.cpp
	$(CXX) -c $(CXXFLAGS) test.cpp -o test.o
