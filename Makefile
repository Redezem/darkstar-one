all:assign

assign: assignment.o Cube.o Sphere.o
	g++ assignment.o Cube.o -o assign -lglut -lGLU -lGL

assignment.o: assignment.cpp Cube.h
	g++ -c assignment.cpp

Cube.o: Cube.cpp Cube.h
	g++ -c Cube.cpp

Sphere.o: Sphere.cpp Sphere.h
	g++ -c Sphere.cpp

clean: 
	rm *.o assign
