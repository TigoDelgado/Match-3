all: development

production:
	g++ -Iinclude -Llib -o bin/Game src/*.cpp -O3 -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

development:
	g++ -Iinclude -Llib -o debug/Game_debug src/*.cpp -g -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image