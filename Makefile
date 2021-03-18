CFLAGS = -std=c++17
LDFLAGS = -lstdc++ -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
	
main:
	gcc $(CFLAGS) src/*.cpp -o game.run $(LDFLAGS)

clear:
	rm main