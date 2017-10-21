#include "ZorkGame.hpp"


ZorkGame::ZorkGame(char* fileName) {

	xml_document<> doc;
	xml_node<> * root_node;

	// Read the xml file into a vector
	ifstream theFile(fileName);
	if (!theFile.is_open()) {
		cout << "Error opening file.... Exiting." << endl;
	}

	//Arrays that contain xml nodes
	std::vector<xml_node<>*> roomx;
    std::vector<xml_node<>*> containerx;
    std::vector<xml_node<>*> itemx;
    std::vector<xml_node<>*> creaturex;

	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);

	// Find root node
	root_node = doc.first_node();
    
    for(xml_node<>*top = root_node -> first_node(); top; top = top -> next_sibling()){
      // std::cout << top->name() << std::endl;
      
      if(string(top->name()) == string("room")){
        roomx.push_back(top);
      }
      if(string(top->name()) == string("container")){
        containerx.push_back(top);
      }
      if(string(top->name()) == string("item")){
        itemx.push_back(top);
      }
      if(string(top->name()) == string("creature")){
        creaturex.push_back(top);
      }
    }

    std::cout << "\nPopulate Room Node" << std::endl;
	for (int i = 0; i < roomx.size(); i++){
		Room * room = new Room(roomx[i]);
		roomNodes.push_back(room);
	}

	curr_room = roomNodes.front();

	//Populate Container
	// std::cout << "\nPopulate Container Node" << std::endl;
	// for (int i = 0; i < containerx.size(); i++){
	// 	Container * container = new Container(containerx[i]);
	// 	containerNodes.push_back(container);
	// }

	//Populate Item 
	// std::cout << "\nPopulate Item Node" << std::endl;
	// for (int i = 0; i < itemx.size(); i++){
	// 	Item * item = new Container(itemx[i]);
	// 	itemNodes.push_back(item);
	// }

	//Populate Creature
	// std::cout << "\nPopulate Creature Node" << std::endl;
	// for (int i = 0; i < creaturex.size(); i++){
	// 	Creature * room = new Creature(creaturex[i]);
	// 	creatureNodes.push_back(room);
	// }
}

void ZorkGame::startGame()
{
	isGameOver = false;
	showRoomDescription();

	while(!isGameOver) {
		getline(cin, userIn);

		if (userIn == "quit"){
			break;
		}

		checkUserInput(); //Check to see what action needs to be performed
		userIn = ""; //Clear user input for next iteration
	}
}

void ZorkGame::checkUserInput()
{
	if(userIn == "n" || userIn == "s" || userIn == "e" || userIn == "w") {
		change_room(userIn);
	}
}

void ZorkGame::change_room(string d)
{
	Room * nextRoom = curr_room;
	//NEED BORDER CLASS!
}


void ZorkGame::showRoomDescription()
{
	cout << curr_room -> description << endl;

	//Display items

	//Display containers 

	//Display creatures 

	//Display borders
}

ZorkGame::~ZorkGame() {
}

