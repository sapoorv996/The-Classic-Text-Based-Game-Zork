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
        isOverriden = trigger_check();
        if (isOverriden) {
            continue;
        }

        getline(cin, userIn);

        if (userIn == "quit"){
            break;
        }

        isOverriden = trigger_check();
        //cout << "Overide after user in: " << isOverriden << endl;
        if (isOverriden) {
            continue;
        }

        checkUserInput_util(); //Check to see what action needs to be performed
        userIn = ""; //Clear user input for next iteration
    }

    // cout << "end Game";
    return;
}

void ZorkGame::checkUserInput_util()
{
    checkUserInput(userIn);
}

void ZorkGame::checkUserInput(string input)
{
    if (input != userIn){
  		if (input.find("Add ") != string::npos && input.find("to ") != string::npos){
  			Add(input);
  		}else if (input.find("Delete ") != string::npos){
  			Delete(input);
  		}else if (input.find("Update ") != string::npos && input.find("to ") != string::npos){
  			Update(input);
  		}else if (input == "Game Over"){
  			gameOver();
  		}
  	}
    //check for new input
    if(input == "n" || input == "s" || input == "e" || input == "w") {
        change_room(input);
    }

    //display inventory
    else if(input == "i") {
        show_inventory();
    }

    //take item from room
    else if(input.find("take ") != string::npos) {
        string command = input.substr(5);
        take(command);
    }

    //Check for open or open exit input
    else if (input.find("open ") != string::npos){
        string ctnr = input.substr(5);
        open(ctnr);
    }

    //check if the input is read
    else if (input.find("read ") != string::npos){
        string itemToRead = input.substr(5);
        read(itemToRead);
    }

    //check if dropping an item
    else if (input.find("drop ") != string::npos){
        string itemToDrop = input.substr(5);
        drop(itemToDrop);
    }


    //check if putting an item
    else if (input.find("put ") != string::npos && input.find("in ") != string::npos){
        int found = input.find("in ");
        string command = input.substr(4, found-5);
        string command2 = input.substr(found+3);

        put(command, command2);
    }

    else if (input.find("turn on") != string::npos){
        //1. Find item in string
        int found = input.find("on ");
        string item = input.substr(found + 3);
        //2. Call turnon
        turnon(item);
    }

    else if (input.find("attack") != string::npos){
        //1. Find creature in string
        int found = input.find("with ");
        //check if input is valid
        if (found == string::npos) {
          cout << "Error" << endl;
          return;
        }
        string creature = input.substr(7, found - 8);
        //2. Find item in string
        string item = input.substr(found + 5);
        //3. Call attack
        attack(creature, item);
    }

    else {
        if (userIn == input) {
          cout << "Error" << endl;
        }
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
                    //TODO: add container items to room
                    for (unsigned int k = 0; k < containerNodes[j]->item_list.size()-1; k++){
                        string itemToAdd = containerNodes[j]->item_list[k];
                        cout << itemToAdd << ", ";
                        curr_room->item_arr.push_back(itemToAdd);
                    }
                    cout << containerNodes[j]->item_list.back() << endl;
                    curr_room->item_arr.push_back(containerNodes[j]->item_list.back());
                    showRoomDescription();
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
        cout << "Can't go that way!" << endl;
    }else{
        curr_room = nextRoom;
        cout << "move to room " + curr_room->name << endl;
        showRoomDescription();
    }

    // showRoomDescription();
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

void ZorkGame::turnon(string item) {
    //1. Check if item exists
    if (whatIs(item) != "Item") {
        std::cout << "Error" << std::endl;
        return;
    }
    int i = whichOne(item, itemNodes);
    //2. Check if the item is in the inventory
    if (std::find(inventory.begin(), inventory.end(), item) == inventory.end()) {
        std::cout << "Error" << std::endl;
        return;
    }
    //3. Check if item can be turned on
    if (!(itemNodes[i]->turnon.has)) {
        std::cout << "Error" << std::endl;
        return;
    }
    //4. Turn on item
    std::cout << "You activated the " << itemNodes[i]->name << "." << std::endl;
    std::cout << itemNodes[i]->turnon.print << std::endl;
    performAction(itemNodes[i]->turnon.action);
    return;
}

void ZorkGame::performAction(string action) {
    //cout << "Performing action: " << action << endl;
    if(action.find("Add") != string::npos){
        Add(action);
        return;
    }
    if(action.find("Delete") != string::npos){
        Delete(action);
        return;
    }
    if(action.find("Update") != string::npos){
      //cout << "Calling update..." << endl;
        Update(action);
        return;
    }
    if(action == "Game Over"){
        gameOver();
        return;
    }
    userIn = action;
    checkUserInput_util();
}

void ZorkGame::attack(string creature, string item) {
    int i;
    //cout << "attack called" << endl;
    //1. Check if the creature is in the room
    for (i = 0; i < curr_room->creature_arr.size(); i++) {
        if (curr_room->creature_arr[i] == creature) {break;}
    }
    if(i == curr_room->creature_arr.size()) {
      cout << "Error" << endl;
      return;
    }
    if (curr_room->creature_arr[i] != creature) {
      std:cout << "Error" << std::endl;
      return;
    }
    //2. Check if the item is in your inventory
    for (i = 0; i < inventory.size(); i++) {
        if (inventory[i] == item) {break;}
    }
    if (inventory[i] != item) {
        std::cout << item << "Error" << std::endl;
        return;
    }
    //3. Start the attack
    std::cout << "You assault the " << creature << " with the " << item << "." << std::endl;
    //4. Check if creature is vulnerable to item
    int whichCreature = whichOne(creature, creatureNodes);
    if (whichCreature == -1) {
      cout << "Error" << std::endl;
      return;
    }
    for (i = 0; i < creatureNodes[whichCreature]->vulnerability.size(); i++) { //for each vulnerability
        if (creatureNodes[whichCreature]->vulnerability[i] == item) {break;}
    }
    if (i == creatureNodes[whichCreature]->vulnerability.size()) {
        std::cout << "It's not very effective." << std::endl;
        return;
    }
    //5. Check if there is an effect of the attack
    if (creatureNodes[whichCreature]->attack == NULL) {return;}
    //6. Check conditions
    if (creatureNodes[whichCreature]->attack->condition != NULL) {
        Status condStat = creatureNodes[whichCreature]->attack->condition->status;
        Owner condOwner = creatureNodes[whichCreature]->attack->condition->owner;
        if (condStat.status != "") { //Condition fields: object and status
            int loc;
            loc = whichOne(condStat.object, roomNodes); //check if type of status.object is room
            if (loc != -1) {roomNodes[loc]->status = condStat.status;}
            loc = whichOne(condStat.object, containerNodes); //check if type of status.object is container
            if (loc != -1) {containerNodes[loc]->status = condStat.status;}
            loc = whichOne(condStat.object, itemNodes); //check if type of status.object is item
            if (loc != -1) {itemNodes[loc]->status = condStat.status;}
            loc = whichOne(condStat.object, creatureNodes); //check if type of status.object is creature
            if (loc != -1) {creatureNodes[loc]->status = condStat.status;}
        }
    }
    //7. Print the attack
    if (creatureNodes[whichCreature]->attack->has_print) {
        std::cout << creatureNodes[whichCreature]->attack->print << std::endl;
    }
    //8. Perform the actions
    for (i = 0; i < creatureNodes[whichCreature]->attack->action.size(); i++) { //for each action
        performAction(creatureNodes[whichCreature]->attack->action[i]);
    }
}

void ZorkGame::Add(string action) {
    //Usage: Add (object) to (room/container)
    int found = action.find("to ");
    //1. Get substrings
    string room_container = action.substr(found + 3);
    string object = action.substr(4, found - 5);
    //2. Get the type of the object (can be Container, Item, or Creature in this case)
    string objectType = whatIs(object);
    //3. Add the object to the appropriate arrray
    if (objectType == "Container" || objectType == "Creature") {
        int loc = whichOne(room_container, roomNodes);
        roomNodes[loc]->container_arr.push_back(object);
        return;
    }
    if (objectType == "Item") {
        if (whatIs(room_container) == "Room") { //check if we have a room or container
            int loc = whichOne(room_container, roomNodes);
            roomNodes[loc]->item_arr.push_back(object);
            return;
        } else {
            int loc = whichOne(room_container, containerNodes);
            containerNodes[loc]->item_list.push_back(object);
            return;
        }
    }
}

void ZorkGame::Delete(string object) { //Delete references in each room should "delete" the item...
    //Source: https://stackoverflow.com/questions/991335/how-to-erase-delete-pointers-to-objects-stored-in-a-vector
    //Usage: Delete (object)
    object = object.substr(7);
    int i, j;
    for (i = 0; i < roomNodes.size(); i++) { //for each room
        for (j = 0; j < roomNodes[i]->border_arr.size(); j++) { //for each border
            if (roomNodes[i]->border_arr[j]->name == object) {
                roomNodes[i]->removeBorder(roomNodes[i]->border_arr[j]->name);
            }
        }
        vector<string>::iterator s;
        for(s = roomNodes[i]->container_arr.begin(); s != roomNodes[i]->container_arr.end();) { //for each container
            if (*s == object) {
                s = roomNodes[i]->container_arr.erase(s);
            } else {s++;}
        }
        for(s = roomNodes[i]->item_arr.begin(); s != roomNodes[i]->item_arr.end();) { //for each item
            if (*s == object) {
                s = roomNodes[i]->item_arr.erase(s);
            } else {s++;}
        }
        for(s = roomNodes[i]->creature_arr.begin(); s != roomNodes[i]->creature_arr.end();) { //for each creature
            if (*s == object) {
                s = roomNodes[i]->creature_arr.erase(s);
            } else {s++;}
        }
    }
}

void ZorkGame::Update(string action) {
    //Usage: Update (object) to (status)
    int found = action.find("to ");
    string object = action.substr(7, found - 8);
    string status = action.substr(found + 3);
    //cout << "Update: object = " << object << ", status = " << status << endl;
    int i;
    for (i = 0; i < roomNodes.size(); i++) { //for each room
        if (roomNodes[i]->name == object) {
            roomNodes[i]->status = status;
            return;
        }
    }
    for (i = 0; i < containerNodes.size(); i++) { //for each container
        if (containerNodes[i]->name == object) {
            containerNodes[i]->status = status;
            return;
        }
    }
    for (i = 0; i < itemNodes.size(); i++) { //for each item
        if (itemNodes[i]->name == object) {
            itemNodes[i]->status = status;
            //cout << "Updated " << itemNodes[i]->name << " to " << itemNodes[i]->status << endl;
            return;
        }
    }
    for (i = 0; i < creatureNodes.size(); i++) { //for each creature
        if (creatureNodes[i]->name == object) {
            creatureNodes[i]->status = status;
            return;
        }
    }
}

string ZorkGame::whatIs(string name) {
    if (whichOne(name, roomNodes) != -1) {return "Room";}
    if (whichOne(name, containerNodes) != -1) {return "Container";}
    if (whichOne(name, itemNodes) != -1) {return "Item";}
    if (whichOne(name, creatureNodes) != -1) {return "Creature";}
    return "";
}

template <class T> int ZorkGame::whichOne(string name, vector<T*> vec) {
    int i = 0;
    while (i < vec.size()) { //go through each item in game
        //check if the item is the one we're looking for
        if (vec[i]->name == name) {return i;}
        i++;
    }
    return -1;
}

ZorkGame::~ZorkGame() {
}

bool ZorkGame::trigger_check(){
    //for each trigger in the current room
    for (int j = 0; j < curr_room->trigger_arr.size(); j++){
        if (trigger_condition_met(curr_room->trigger_arr[j])){
            return true;
        }
    }
    // cout << "NO ROOM TRIGGER" << endl;
    //for each item in the room
    int whichItem;
    for (int i = 0; i < curr_room->item_arr.size(); i++){
        whichItem = whichOne(curr_room->item_arr[i], itemNodes); //find the index of the item
        for (int j = 0; j < itemNodes[whichItem]->trigger_list.size(); j++){ //for each trigger in item
            if (trigger_condition_met(itemNodes[whichItem]->trigger_list[j])){ //check if its condition is met
                return true;
            }
        }
    }
    // cout << "NO ITEM TRIGGER" << endl;
    //for each container in the room
    int whichContainer;
    for (int i = 0; i < curr_room->container_arr.size(); i++) {
        // cout << "Current Room: " << curr_room->name << endl;
        // cout << "Creature: " << curr_room->creature_arr[i] << endl;
        whichContainer = whichOne(curr_room->container_arr[i], containerNodes); //find the index of the creature
        //cout << "Container index: " << whichContainer;
        //cout << ", Number of triggers: " << containerNodes[whichContainer]->trigger_list.size() << endl;
        for (int j = 0; j < containerNodes[whichContainer]->trigger_list.size(); j++){
          //cout << "trigger_condition_met called for container" << endl;
            if (trigger_condition_met(containerNodes[whichContainer]->trigger_list[j])){
              //cout << "trigger_condition_met for container" << endl;
                return true;
            }
        }
    }
    // cout << "NO CONTA TRIGGER" << endl;
    //for each creature in the room
    int whichCreature;
    for (int i = 0; i < curr_room->creature_arr.size(); i++) {
        // cout << "Current Room: " << curr_room->name << endl;
        // cout << "Creature: " << curr_room->creature_arr[i] << endl;
        whichCreature = whichOne(curr_room->creature_arr[i], creatureNodes); //find the index of the creature
        // cout << "Creature index: " << whichCreature;
        // cout << ", Number of triggers: " << creatureNodes[whichCreature]->trigger_list.size() << endl;
        for (int j = 0; j < creatureNodes[whichCreature]->trigger_list.size(); j++){
            if (trigger_condition_met(creatureNodes[whichCreature]->trigger_list[j])){
              //cout << "trigger_condition_met for creature" << endl;
                return true;
            }
        }
    }
    // cout << "NO CREA TRIGGER" << endl;

    return false;
}

bool ZorkGame::trigger_condition_met(Trigger* trigger){
    bool is_triggered = false;
    Status * s = NULL;
    Owner * o = NULL;

    for(int i = 0; i < trigger->conditions.size(); i++) {
        // cout << "just once" << endl;
        if(trigger->conditions[i]->numConditions == 2) {
            s = &(trigger->conditions[i]->status);
        } else if(trigger->conditions[i]->numConditions == 3) {
            o = &(trigger->conditions[i]->owner);
            // cout << "has = " << o->has << endl;
            // cout << "owner = " << o->owner << endl;
            // cout << "object = " << o->object << endl;
        }
    }

    // cout << "THE TRIGGER COMMAND IS: " << trigger->command << endl;

    if (trigger->command == ""){
        //cout << "i have no command" << trigger->command << endl;
        if (o && s){
            if (trig_owner_met(trigger, o) && trig_status_met(trigger, s)){
                is_triggered = true;
            }
        }
        else if (s){
            if (trig_status_met(trigger, s)){
                is_triggered = true;
            }
        }
        else if (o){
          //cout << "trig_owner_met called" << endl;
            if (trig_owner_met(trigger, o)){
                is_triggered = true;
            }
        }

    // cout << "no command" << endl;

    }else{
        // cout << "i have command" << trigger->command << endl;
        if (trigger->command != userIn){
            return false;
        }

        // cout << "check all" << endl;
        if ((o != NULL) && (s != NULL)){
            if (trig_owner_met(trigger, o) && trig_status_met(trigger, s)){
                is_triggered = true;
            }
        }
        else if (s != NULL){
            if (trig_status_met(trigger, s)){
                is_triggered = true;
            }
        }
        else if (o != NULL){
             //cout << "I SHOULD BE HERE - CHECKING OWNER IN 1" << endl;
            if (trig_owner_met(trigger, o)){
                is_triggered = true;
            }
        }
    }

    if (is_triggered){
        implement_trigger(trigger);
        return true;
    }

    //cout << "for real?" << endl;

    return false;
}

void ZorkGame::implement_trigger(Trigger* trigger){

    cout << trigger->print << endl;
    // cout << "Trigger type = " << trigger->type << endl;
    // cout << "trigger->action.size() = " << trigger->action.size() << endl;
    for (unsigned int j = 0; j < trigger->action.size(); j++){
        //cout << "trigger->action[j] = " << trigger->action[j] << endl;
        checkUserInput(trigger->action[j]);
    }

    if (trigger->type == "single" || trigger->type == "") {
        trigger->type = "used";
    }
    if (trigger->type == "permanent") {
        trigger->type = "permanent - used";
    }
}

bool ZorkGame::trig_owner_met(Trigger* trigger, Owner * o){
    if (o){
        string owner = o->owner;
        string obj = o->object;
        //cout << "owner_has_object = " << owner_has_object(owner, obj) << endl;
        if ((o->has == true && owner_has_object(owner, obj))
                || (o->has == false && !owner_has_object(owner, obj))){
            if (trigger->type != "used" && trigger->type != "permanent - used") {
              //cout << "owner met true" << endl;
                return true;
            }
            if (trigger->type == "permanent - used") {
              trigger->type = "permanent";
            }
        }
    }

    return false;
}

bool ZorkGame::trig_status_met(Trigger* trigger, Status * s){
     if (s){
        string status = s->status;
        string obj = s->object;
        if (object_meet_status(obj, status)){
          if (trigger->type != "used" && trigger->type != "permanent - used") {
              return true;
          }
          if (trigger->type == "permanent - used") {
            trigger->type = "permanent";
          }
        }
    }

    return false;
}

bool ZorkGame::owner_has_object(string owner, string obj){
    string owner_type = ztype(owner);
    string obj_type = ztype(obj);
    // cout << "owner is " << owner << endl;
    // cout << "obj is " << obj << endl;
    // cout << "owner_type is " << owner_type << endl;
    // cout << "obj_type is " << obj_type << endl;

    if (owner_type == "inventory"){
      //cout << "owner_has_object inventory" << endl;
      //cout << "obj_type = " << obj_type <<endl;
        if (obj_type == "item"){
            for (unsigned int i = 0; i < inventory.size(); i++){
                if (inventory[i] == obj){
                    return true;
                }
            }
        }
    }else if (owner_type == "room"){
        if (obj_type == "item"){
            for (unsigned int i = 0; i < roomNodes.size(); i++){
                if (roomNodes[i]->name == owner){
                    return roomNodes[i]->has_item(obj);
                }
            }
        }else if (obj_type == "container"){
            for (unsigned int i = 0; i < roomNodes.size(); i++){
                if (roomNodes[i]->name == owner){
                    return roomNodes[i]->has_container(obj);
                }
            }
        }else if (obj_type == "creature"){
            for (unsigned int i = 0; i < roomNodes.size(); i++){
                if (roomNodes[i]->name == owner){
                    return roomNodes[i]->has_creature(obj);
                }
            }
        }
    }else if (owner_type == "container"){
        if (obj_type == "item") {
            for (unsigned int i = 0; i < containerNodes.size(); i++){
              //cout << "roomNodes[" << i << "]->name = " << containerNodes[i]->name << endl;
                if (containerNodes[i]->name == owner){
                    //cout << "roomNodes[i]->name = " << containerNodes[i]->name << endl;
                    return containerNodes[i]->has_item(obj);
                }
            }
        }
    }

    return false;
}

bool ZorkGame::object_meet_status(string obj, string status){
    string obj_type = ztype(obj);

    if (obj_type == "item"){
        for (unsigned int i = 0; i < itemNodes.size(); i++){
            if (itemNodes[i]->name == obj){
                if (itemNodes[i]->status == status){
                    return true;
                }
            }
        }
    }
    else if (obj_type == "room"){
        for (unsigned int i = 0; i < roomNodes.size(); i++){
            if (roomNodes[i]->name == obj){
                if (roomNodes[i]->status == status){
                    return true;
                }
            }
        }
    }
    else if (obj_type == "container"){
        for (unsigned int i = 0; i < containerNodes.size(); i++){
            if (containerNodes[i]->name == obj){
                if (containerNodes[i]->status == status){
                    return true;
                }
            }
        }
    }
    else if (obj_type == "creature"){
        for (unsigned int i = 0; i < creatureNodes.size(); i++){
            if (creatureNodes[i]->name == obj){
                if (creatureNodes[i]->status == status){
                    return true;
                }
            }
        }
    }

    return false;
}

string ZorkGame::ztype(string obj){

    if (obj == "inventory"){
        //cout << "ztype: inventory" << endl;
        return "inventory";
    }

    for (unsigned int i = 0; i < itemNodes.size(); i++){
        if (itemNodes[i]->name == obj){
            return "item";
        }
    }
    for (unsigned int i = 0; i < creatureNodes.size(); i++){
        if (creatureNodes[i]->name == obj){
            return "creature";
        }
    }
    for (unsigned int i = 0; i < containerNodes.size(); i++){
        if (containerNodes[i]->name == obj){
            return "container";
        }
    }
    for (unsigned int i = 0; i < roomNodes.size(); i++){
        if (roomNodes[i]->name == obj){
            return "room";
        }
    }
    return "";
}
