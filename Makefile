main: main.o glad.o shader.o stb_image.o camera.o model.o
	g++ -Iinclude -o main main.o glad.o shader.o stb_image.o camera.o model.o -lglfw -lassimp

main.o: src/main.cpp include/glad/glad.h include/shader.h include/camera.h include/model.h
	g++ -Iinclude -c src/main.cpp

glad.o: src/glad.c include/glad/glad.h
	g++ -Iinclude -c src/glad.c

shader.o: src/shader.cpp include/shader.h
	g++ -Iinclude -c src/shader.cpp

stb_image.o: src/stb_image.cpp include/stb_image.h
	g++ -Iinclude -c src/stb_image.cpp

camera.o: src/camera.cpp include/camera.h
	g++ -Iinclude -c src/camera.cpp

model.o: src/model.cpp include/model.h
	g++ -Iinclude -c src/model.cpp

clean:
	rm -f *.o main
