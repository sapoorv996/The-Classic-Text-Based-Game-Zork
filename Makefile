Zork: main.o ZorkGame.o Room.o
	g++ -o Zork main.o ZorkGame.o

main.o: main.cpp ZorkGame.hpp 
	g++ -c main.cpp

ZorkGame.o: Room.o ZorkGame.cpp ZorkGame.hpp
	g++ -c ZorkGame.cpp
	
Room.o: Room.hpp Room.cpp
	g++ -c Room.cpp
		
clean:
	rm *.o Zork