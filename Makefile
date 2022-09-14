FLAGS = -g -Wall -Wpedantic  -std=c++11
CFLAGS = -c $(FLAGS)
LIBS = 
CC = g++
SRC = src
BIN = bin
OBJ = obj
OBJS = DimVecAnalyser.o Python.o DimVecBank.o DimVecLayer.o DimVecSpace.o DimVec.o DimVecArms.o PartitionAlg.o Partition.o
OUTPUT = 
MAIN = main.c $(OUTPUT)


all: $(MAIN)
	$(CC) -o rootbank $(FLAGS) $(MAIN) $(LIBS)


Partition.cpp: Partition.h
	
PartitionAlg.cpp: Partition.h

DimVecArms.cpp: DimVec.h Partition.h

DimVec.cpp: DimVec.h Partition.h

DimVecSpace.cpp: DimVecSpace.h DimVec.h

DimVecLayer.cpp: DimVecLayer.h DimVecSpace.h

DimVecBank.cpp: DimVecBank.h  DimVecLayer.h

DimVecAnalyser.cpp: DimVecAnalyser.h DimVecLayer.h

Python.cpp: Python.h DimVecAnalyser.h

$(OBJ)/%.o: %.cpp
	$(CC) -o $@ $(CFLAGS) $< $(LIBS)

clean:
	rm -f $(OBJ)/%.o

