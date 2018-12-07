# Project: gofish
# Makefile created by Dev-C++ 4.9.9.2
# Modified by jwebb

CPP  = g++
RES  = 
OBJ  = main.o $(RES)
LINKOBJ  = main.o $(RES)
BIN  = plagiarismCatcher
CXXFLAGS = $(CXXINCS)  
CFLAGS = $(INCS)  
RM = rm -f


all: plagiarismCatcher

clean: 
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CPP) $(LINKOBJ) -o plagiarismCatcher $(LIBS)

main.o: main.cpp
	$(CPP) -c main.cpp -o main.o $(CXXFLAGS)
