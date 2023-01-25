all: development

production:
	cd Match-3 && g++ -Iinclude -Isrc -Llib -o bin/Game src/*.cpp src/Systems/*.cpp src/Scenes/*.cpp -O3 -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

development:
	cd Match-3 && g++ -Iinclude -Isrc -Llib -o debug/Game_debug src/*.cpp src/Systems/*.cpp src/Scenes/*.cpp -g -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image


run: runDev

runPro:
	cd Match-3\bin && .\Game.exe

runDev:
	cd Match-3\debug && .\Game_debug.exe