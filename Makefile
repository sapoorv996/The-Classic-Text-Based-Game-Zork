Zork: main.o ZorkGame.o
	g++ -o Zork main.o ZorkGame.o

main.o: main.cpp ZorkGame.hpp 
	g++ -c main.cpp

ZorkGame.o: ZorkGame.cpp ZorkGame.hpp
	g++ -c ZorkGame.cpp
	
# Room.o: Room.hpp Room.cpp Creature.hpp Container.hpp Item.hpp
# 	g++ -c Room.cpp

# Creature.o: Creature.hpp Creature.cpp Trigger.hpp
# 	g++ -c Creature.cpp

# Container.o: Container.hpp Container.cpp Trigger.hpp Item.hpp
# 	g++ -c Container.cpp

# Item.o: Item.hpp Item.cpp Trigger.hpp
# 	g++ -c Item.cpp

# Trigger.o: Trigger.hpp Trigger.cpp
# 	g++ -c Trigger.cpp
		
clean:
	rm *.o Zork