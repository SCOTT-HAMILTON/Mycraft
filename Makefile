
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
CLIBS = -std=c++17
PROJECT = Mycraft

all: compile
	g++ main.o Steve.o Entity.o EntityManager.o -o $(PROJECT) $(LIBS)
	rm main.o
	rm Steve.o
	rm Entity.o
	rm EntityManager.o

compile: main.cpp Steve.cpp Entity.cpp EntityManager.cpp
	g++ -c main.cpp $(CLIBS)
	g++ -c Steve.cpp $(CLIBS)
	g++ -c Entity.cpp $(CLIBS)
	g++ -c EntityManager.cpp $(CLIBS)
