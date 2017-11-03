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
	std::cout << "\nPopulate Container Node" << std::endl;
	for (int i = 0; i < containerx.size(); i++){
		Container * container = new Container(containerx[i]);
		containerNodes.push_back(container);
	}

	//Populate Item 
	std::cout << "\nPopulate Item Node" << std::endl;
	for (int i = 0; i < itemx.size(); i++){
		Item * item = new Item(itemx[i]);
		itemNodes.push_back(item);
	}

	// Populate Creature
	std::cout << "\nPopulate Creature Node" << std::endl;
	for (int i = 0; i < creaturex.size(); i++){
		Creature * creature = new Creature(creaturex[i]);
		creatureNodes.push_back(creature);
	}
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
	//check for nsew input
	if(userIn == "n" || userIn == "s" || userIn == "e" || userIn == "w") {
		change_room(userIn);
	}

	//display inventory
	else if(userIn == "i") {
		show_inventory();
	} 

	//take item from room
	else if(userIn.find("take ") != string::npos) {
		string command = userIn.substr(5);
		take(command);
	}

	//Check for open or open exit input
	else if (userIn.find("open ") != string::npos){
		string ctnr = userIn.substr(5);
		open(ctnr);
	}

	//check if the input is read
	else if (userIn.find("read ") != string::npos){
		string itemToRead = userIn.substr(5);
		read(itemToRead);
	}

	//check if dropping an item
	else if (userIn.find("drop ") != string::npos){
		string itemToDrop = userIn.substr(5);
		drop(itemToDrop);
	}


	//check if putting an item
	else if (userIn.find("put ") != string::npos && userIn.find("in ") != string::npos){
		int found = userIn.find("in ");
		string command = userIn.substr(4, found-5);
		string command2 = userIn.substr(found+3);

		put(command, command2);
	}
}

void ZorkGame::gameOver()
{
	isGameOver = true;
	cout << "Victory!" << endl;
}

void ZorkGame::put(string putItem, string inContainer){
	for (int i = 0; i < inventory.size(); i++){
		if (putItem == inventory[i]){
			for (int j = 0; j < curr_room->container_arr.size(); j++){
				if (inContainer == curr_room->container_arr[j]){
					for (int k = 0; k < containerNodes.size(); k++){
						if (containerNodes[k]->name == inContainer){
							if (!containerNodes[k]->is_accepted_by_container(putItem)){
								cout << putItem << " cannot be put into " << inContainer << endl;
								return;
							}
							containerNodes[k]->item_list.push_back(putItem);
							remove_from_vec(inventory, putItem);
							cout << putItem << " has been put in " << inContainer << endl;
							containerNodes[k]->status = "unlocked";
							return;
						}
					}
				}
			}
			cout << "There is no such container to put items in." << endl;
			return;
		}
	}

	cout << "There is no such item to put" << endl;
	return;
}

void ZorkGame::drop(string dropItem){
	for (unsigned int i = 0; i < inventory.size(); i++){
		if (inventory[i] == dropItem){
			remove_from_vec(inventory, dropItem);
			curr_room->item_arr.push_back(dropItem);
			cout << dropItem << " has been dropped" << endl;
			return;
		}
	}

	cout << "There is no such item to drop" << endl;
	return;
}

void ZorkGame::read(string readItem){
	for (int i = 0; i < inventory.size(); i++){
		if (readItem == inventory[i]){
			for (int j = 0; j < itemNodes.size(); j++){
				if (itemNodes[j]->name == readItem){
					cout << itemNodes[j]->writing << endl;
					return;
				}
			}
		}
	}

	cout << "There is no such item to read" << endl;
	return;
}

void ZorkGame::open(string ctnr)
{
	if (ctnr == "exit" && curr_room->type == "exit") {
		gameOver();
		return;
	}

	for (int i = 0; i < curr_room->container_arr.size(); i++){
		if (curr_room->container_arr[i] == ctnr){
			for (unsigned int j = 0; j < containerNodes.size(); j++){
				if (containerNodes[j]->name == ctnr){
					if (containerNodes[j]->valid_list.size() != 0 && containerNodes[j]->status == "locked"){
						cout << ctnr << " is locked" << endl;
						cout << "You need ";
						for (unsigned int k = 0; k < containerNodes[j]->valid_list.size()-1; k++){
							cout << containerNodes[j]->valid_list[k] << ", ";
						}
						cout << containerNodes[j]->valid_list.back();
						cout << " to open it" << endl;
						return;
					}

					containerNodes[j]->status = "unlocked";

					if (containerNodes[j]->item_list.size() == 0){
						cout << ctnr << " is empty" << endl;
						return;
					}
					cout << ctnr << " contains ";
					for (unsigned int k = 0; k < containerNodes[j]->item_list.size()-1; k++){
						cout << containerNodes[j]->item_list[k] << ", ";
					}
					cout << containerNodes[j]->item_list.back() << endl;
					return;
				}
			}
		}
	}

	cout << "There is no such container in the room! " << endl;

}

void ZorkGame::take(string itemToTake)
{
	for (int i = 0; i < curr_room->item_arr.size(); i++) {
		if (curr_room->item_arr[i] == itemToTake) {
			inventory.push_back(itemToTake);
			remove_from_vec(curr_room->item_arr, i);
			cout << itemToTake << " has been added to inventory" << endl;
			return;
		}
	}

	cout << "There is no such item to take! " << endl;
}

void ZorkGame::remove_from_vec(vector<string> &vec, int index)
{
	vec.erase(vec.begin() + index);
}

void ZorkGame::remove_from_vec(vector<string> &vec, string str){
	for (unsigned int i = 0; i < vec.size(); i++){
		if (vec[i] == str){
			remove_from_vec(vec, i);
		}
	}
}


void ZorkGame::show_inventory()
{
	if(inventory.size() == 0) {
		cout << "Inventory: empty" << endl;
	}

	else {
		cout << "Inventory: ";
		for (int i = 0; i < inventory.size()-1; i++) {
			cout << inventory[i] + ", ";
		}

		cout << inventory[inventory.size()-1];
		cout << endl;
	}
}

void ZorkGame::change_room(string d)
{
	Room * nextRoom = curr_room;

	for (int i = 0; i < curr_room->border_arr.size(); i++){
		if (curr_room->border_arr[i]->direction == d){
			for (int j = 0; j < roomNodes.size(); j++){
				if (roomNodes[j]->name == curr_room->border_arr[i]->name){
					nextRoom = roomNodes[j];
					break;
				}
			}
			break;
		}
	}
	if (nextRoom == curr_room){
		cout << "can't go that way!" << endl;
	}else{
		curr_room = nextRoom;
		cout << "move to room " + curr_room->name << endl;
	}

	showRoomDescription();
}


void ZorkGame::showRoomDescription()
{
	cout << curr_room -> description << endl;

	//Display items
	if (curr_room->item_arr.size() != 0){
		cout << "Items in this room: ";
		for (int i = 0; i < curr_room->item_arr.size()-1; i++){
			cout << curr_room->item_arr[i] << ", ";
		}
		cout << curr_room->item_arr.back();
		cout << endl;
	}

	//Display containers
	if (curr_room->container_arr.size() != 0){
		cout << "Containers in this room: ";
		for (int i = 0; i < curr_room->container_arr.size()-1; i++){
			cout << curr_room->container_arr[i] << ", ";
		}
		cout << curr_room->container_arr.back();
		cout << endl;
	} 

	//Display creatures 
	if (curr_room->creature_arr.size() != 0){
		cout << "Creatures in this room: ";
		for (int i = 0; i < curr_room->creature_arr.size()-1; i++){
			cout << curr_room->creature_arr[i] << ", ";
		}
		cout << curr_room->creature_arr.back();
		cout << endl;
	}

	//Display borders
	if (curr_room->border_arr.size() != 0){
		cout << "You may go ";
		for (int i = 0; i < curr_room->border_arr.size()-1; i++){
			cout << curr_room->border_arr[i] -> direction << ", ";
		}
		cout << curr_room->border_arr.back()->direction;
		cout << endl;
	}
}

ZorkGame::~ZorkGame() {
}

