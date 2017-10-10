#ifndef ZORKGAME_H_
#define ZORKGAME_H_

#include <iostream>
#include <string>
#include <stdio.h>
#include "rapidxml.hpp"
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <queue>  
#include "Room.hpp"
#include <sstream>
#include <iterator>

using namespace rapidxml;
using namespace std;

class ZorkGame
{
public:
	ZorkGame(char*);
	~ZorkGame();

	std::vector<xml_node<>*> roomx;
    std::vector<xml_node<>*> containerx;
    std::vector<xml_node<>*> itemx;
    std::vector<xml_node<>*> creaturex;
	
};

#endif /* ZORKGAME_H_ */