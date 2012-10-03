all:assign

assign: assignment.o Cube.o Sphere.o Polygon.o
	g++ assignment.o Cube.o Sphere.o Polygon.o SOIL/image_DXT.o SOIL/image_helper.o SOIL/stb_image_aug.o SOIL/SOIL.o -o assign -lglut -lGLU -lGL

assignment.o: assignment.cpp Cube.h
	g++ -c assignment.cpp

Cube.o: Cube.cpp Cube.h
	g++ -c Cube.cpp

Sphere.o: Sphere.cpp Sphere.h
	g++ -c Sphere.cpp

Polygon.o: Polygon.cpp Polygon.h
	g++ -c Polygon.cpp

clean: 
	rm *.o assign
