main: main.o glad.o shader.o
	g++ -Iinclude -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -o main main.o glad.o shader.o

main.o: src/main.cpp include/glad/glad.h
	g++ -Iinclude -c src/main.cpp

glad.o: src/glad.c include/glad/glad.h
	g++ -Iinclude -c src/glad.c

shader.o: src/shader.cpp
	g++ -Iinclude -c src/shader.cpp

clean:
	rm -f *.o main
