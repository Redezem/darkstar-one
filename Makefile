all:assign

assign: assignment.o Cube.o Sphere.o Polygon.o TextureLoader.o
	g++ assignment.o Cube.o Sphere.o Polygon.o TextureLoader.o SOIL/image_DXT.o SOIL/image_helper.o SOIL/stb_image_aug.o SOIL/SOIL.o -o assign -lglut -lGLU -lGL
	cp Imurges/Betterfloor.jpg Betterfloor.jpg
	cp Imurges/Betterwall.jpg Betterwall.jpg
	cp Imurges/borg.jpg borg.jpg
	cp Imurges/floor.png floor.png
	cp Imurges/World.jpg World.jpg

assignment.o: assignment.cpp Cube.h
	g++ -c assignment.cpp

Cube.o: Cube.cpp Cube.h
	g++ -c Cube.cpp

Sphere.o: Sphere.cpp Sphere.h
	g++ -c Sphere.cpp

Polygon.o: Polygon.cpp Polygon.h
	g++ -c Polygon.cpp

TextureLoader.o: TextureLoader.cpp TextureLoader.h
	g++ -c TextureLoader.cpp

clean: 
	rm *.o *.jpg *.png assign
