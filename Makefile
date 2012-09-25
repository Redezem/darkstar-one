all:assign

assign: assignment.o Cube.o
	g++ assignment.o Cube.o -o assign -lglut -lGLU -lGL

assignment.o: assignment.cpp Cube.h
	g++ -c assignment.cpp

Cube.o: Cube.cpp Cube.h
	g++ -c Cube.cpp

clean: 
	rm *.o assign
