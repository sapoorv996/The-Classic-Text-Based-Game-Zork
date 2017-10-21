#include "ZorkGame.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
	std::cout << "Zork Project - ECE 30862 - Fall 17" << std::endl;
	std::cout << "Authors:" << std::endl << "\tApoorv Sharma" << std::endl << "\tGeoff Cramer" << std::endl;
	
	if(argc < 1) {
		std::cout << "Error! Please enter ./Zork [XMLFile]" << std::endl;
	}

	ZorkGame * zork = new ZorkGame(argv[1]); //Parse the xml file
	zork -> startGame(); //Start the zork game

	delete zork;

	return 0;
}