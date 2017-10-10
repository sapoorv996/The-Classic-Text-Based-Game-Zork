#include "ZorkGame.hpp"


ZorkGame::ZorkGame(char* fileName) {

	xml_document<> doc;
	xml_node<> * root_node;

	// Read the xml file into a vector
	ifstream theFile(fileName);
	if (!theFile.is_open()) {
		cout << "Error opening file.... Exiting." << endl;
	}
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	// Parse the buffer using the xml file parsing library into doc 
	doc.parse<0>(&buffer[0]);

	// Find root node
	root_node = doc.first_node();
    
    for(xml_node<>*top = root_node -> first_node(); top; top = top -> next_sibling()){
      std::cout << top->name() << std::endl;
      
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

    // rapidxml::print_node(NULL ,roomx, 0, 0);
    std::cout << root_node -> name() << std::endl;


}

ZorkGame::~ZorkGame() {
}

