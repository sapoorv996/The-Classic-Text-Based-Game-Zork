#ifndef ROOM_H_
#define ROOM_H_

#include <string>
#include <vector>

class Room
{
public:
	std::string name;
	std::string status; 
	std::string type; 
	std::string description; 
	// vector<Border *> border_arr; 
	// vector<std::string *> container_arr; 
	// vector<std::string *> item_arr;
	// vector<std::sting *> creature_arr; 
	// vector<Trigger *> trigger_arr;

	Room();
	~Room();
	
};
#endif /* ROOM_H_ */