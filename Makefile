CFLAGS = -std=c++20 -O2
LDFLAGS = -lstdc++ -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr 
	
main:
	gcc $(CFLAGS) src/*.cpp -o game.run $(LDFLAGS)

debug:
	gcc $(CFLAGS) -g src/*.cpp -o game.run $(LDFLAGS)

clear:
	rm main