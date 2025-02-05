main: main.o glad.o shader.o stb_image.o
	g++ -Iinclude -lglfw -o main main.o glad.o shader.o stb_image.o

main.o: src/main.cpp include/glad/glad.h
	g++ -Iinclude -c src/main.cpp

glad.o: src/glad.c include/glad/glad.h
	g++ -Iinclude -c src/glad.c

shader.o: src/shader.cpp
	g++ -Iinclude -c src/shader.cpp

stb_image.o: src/stb_image.cpp include/stb_image.h
	g++ -Iinclude -c src/stb_image.cpp

clean:
	rm -f *.o main
