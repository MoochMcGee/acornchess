CXX=g++
CFLAGS=-std=c++17 -g
LIBS=
VPATH=src
OBJ = i808x_cpu.o \
ibm5150.o \
main.o

all: $(OBJ)
	$(CXX) $(CFLAGS) $(OBJ) -o acornchess $(LIBS)

clean:
	rm *.o && rm acornchess

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<