all: development

production:
	g++ -Iinclude -Isrc -Llib -o bin/Game src/*.cpp src/Systems/*.cpp -O3 -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

development:
	g++ -Iinclude -Isrc -Llib -o debug/Game_debug src/*.cpp src/Systems/*.cpp -g -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image