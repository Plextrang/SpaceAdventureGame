using namespace std;
#include <iostream>
#include "Planet.h"
#include "Spaceship.h"
#include "TitanHauler.h"
#include "InfinityFueler.h"
#include "NovaCruiser.h"
#include <string>
#include <cmath>
#include <ctime>
#include <random>
#include <vector>
#include <sstream>
#include <fstream>
#include <cstdlib>

const int planetCount = 20; // fixed quantity of planets within the district
const int fuelneeded = 2; // Moving one unit of distance requires two units of fuel.

int NumberSpaceships = 0; // game will end in a loss when 0
int NumberPlanets = 20; //reaches 0, the game will end and the player wins.

void PurchaseSpaceShip(vector<Spaceship*>& inventory, int& balance) {
    const int spaceshipCost = 400000;

    if (balance < spaceshipCost) {
        cout << "You don't have enough coins to purchase this spaceship." << endl;
        return;
    }

    cout << "Select a spaceship to purchase:" << endl;
    cout << "(1) NovaCruiser: 75 crew capacity, 390000 fuel capacity" << endl;
    cout << "(2) TitanHauler: 100 crew capacity, 250000 fuel capacity" << endl;
    cout << "(3) InfinityFueler: 50 crew capacity, 500000 fuel capacity" << endl;
    cout << "Your current balance is " << balance << "." << endl;
    cout << "Do you want to proceed with the purchase? (yes/no): ";

    string answer;
    cin >> answer;

    if (answer != "yes") {
        return;
    }

    int choice;
    cout << "Enter your choice (1/2/3): ";
    cin >> choice;

    while (cin.fail() || choice < 1 || choice > 3) {
        cout << "Invalid choice. Please enter a number between 1 and 3: ";
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> choice;
    }

    Spaceship* newShip = nullptr;

    switch (choice) {
        case 1:
            newShip = new NovaCruiser();
            break;
        case 2:
            newShip = new TitanHauler();
            break;
        case 3:
            newShip = new InfinityFueler();
            break;
    }

    inventory.push_back(newShip);
    NumberSpaceships++;
    balance -= spaceshipCost;

    cout << "Spaceship purchased successfully." << endl;
}

void switchSpaceships(vector<Spaceship*>& spaceships, int index1, int index2){
    Spaceship* tempSpaceship = spaceships[index1];
    spaceships[index1] = spaceships[index2];
    spaceships[index2] = tempSpaceship;
}

int CalculateFuelNeeded(Planet* source, Planet* destination) {
    int distance = pow(destination->x - source->x, 2) + pow(destination->y - source->y, 2);
    int fuel_needed = distance * fuelneeded;
    return fuel_needed;
}

void GeneratePlanets(Planet* arr[]) {
    srand(time(NULL));
    for(int i = 0; i < planetCount; ++i){
        int x = rand() % 3;
        switch(x){
            case 0:
                arr[i] = new Planet(1, i);
                break;
            case 1:
                arr[i] = new Planet(2, i);
                break;
            case 2:
                arr[i] = new Planet(3, i);
                break;
        }
    }
}

void SaveGame(vector<Spaceship*>& arr, int& balance) {
    ofstream outFS("Progress.txt");
    if(!outFS.is_open()) {
        cout << "Unable to open savefile." << endl;
        exit(0);
    }

    outFS << balance << endl;

    for(auto spaceship : arr) {
        outFS << spaceship->getcrewcap() << " " << spaceship->curr_crew << " " << spaceship->curr_fuel << endl;
    }

    outFS.close();
}

void LoadGame(vector<Spaceship*>& arr, Planet* parr[], int& balance) {
    ifstream file("Progress.txt");
    if (!file.is_open()) {
        cerr << "Error: Unable to open Progress.txt. Please check if the file exists or if you have the correct permissions." << endl;
        exit(EXIT_FAILURE);
    }

    // Read the balance from the file
    file >> balance;
    if (file.fail()) {
        cerr << "Error: Unable to read balance from Progress.txt." << endl;
        exit(EXIT_FAILURE);
    }

    // Read each spaceship's data from the file
    while (true) {
        int capacity, crew, fuel;
        file >> capacity >> crew >> fuel;
        if (file.fail()) {
            break;  // stop reading if we've reached the end of the file or encountered an error
        }

        Spaceship* ship;
        if (capacity == 50) {
            ship = new InfinityFueler();
        } else if (capacity == 75) {
            ship = new NovaCruiser();
        } else if (capacity == 100) {
            ship = new TitanHauler();
        } else {
            cerr << "Error: Unknown spaceship capacity in Progress.txt." << endl;
            exit(EXIT_FAILURE);
        }

        ship->curr_crew = crew;
        ship->curr_fuel = fuel;
        ship->setPlanet(parr[NumberPlanets - 1]);  // assign the last planet in parr to the spaceship
        arr.push_back(ship);
        NumberSpaceships++;
        NumberPlanets--;  // decrement NumberPlanets so that the next spaceship gets assigned to the previous planet
    }
    
    file.close();
}

bool EndGameLost(){
    return NumberSpaceships == 0;
} // checks if the game is over and the player lost

bool VictoryScreen(){
    return NumberPlanets == 0;
} // checks if the game is over and player won

void ExplorePlanet(Planet* arr[], Spaceship** spaceship, vector<Spaceship*>& arr2) {
    string option;
    int minFuelNeeded = numeric_limits<int>::max();
    Planet* selectedPlanet = nullptr;

    for (int i = planetCount - 1; i >= 0; i--) {
        Planet* planet = arr[i];
        if (planet == nullptr) {
            continue;
        }

        int fuelNeeded = CalculateFuelNeeded((*spaceship)->getPlanet(), planet);
        if (fuelNeeded >= (*spaceship)->curr_fuel) {
            continue;
        }

        if (fuelNeeded < minFuelNeeded) {
            minFuelNeeded = fuelNeeded;
            selectedPlanet = planet;
        }

        cout << "Do you want to travel to " << planet->getName() << "?" << endl;
        cout << "Level of Difficulty: " << planet->GetHostility() << endl;
        cout << "Enter 'yes' to travel or any other key to skip: ";
        cin >> option;
        if (option == "yes") {
            cout << "Reached the destination " << planet->getName() << endl;
            delete (*spaceship)->getPlanet();
            (*spaceship)->setPlanet(planet);
            (*spaceship)->UseFuel(fuelNeeded);
            NumberPlanets--;
            return;
        }
    }

    if (selectedPlanet == nullptr) {
        cout << "There are no reachable planets." << endl;
        return;
    }

    if ((*spaceship)->curr_fuel < minFuelNeeded) {
        if ((*spaceship)->getPlanet()->eventleft == 0) {
            cout << "The spacecraft finds itself ensnared on the planet's surface with no foreseeable escape, as if a captive bird in a cage with no hope of flight. Its fate appears to be sealed, destined to remain on this planet forever, like a castaway stranded on a deserted island." << endl;
            delete *spaceship;
            *spaceship = nullptr;

            for (int i = 0; i < arr2.size(); ++i) {
                if (arr2.at(i) == nullptr) {
                    switchSpaceships(arr2, i, arr2.size() - 1);
                    arr2.pop_back();
                    NumberSpaceships--;
                    return;
                }
            }
        }
    }

    cout << "You didn't travel to a different planet." << endl;
}

void DeleteSpaceships(vector<Spaceship*>& spaceships) {
    for (auto spaceship : spaceships) {
        if (spaceship != nullptr) {
            delete spaceship;
            spaceship = nullptr;
        }
    }
    cout << "All spaceships have been deleted." << endl;
}

void DeleteSpaceships(Planet* arr[]) {
    for(int i = 0; i < planetCount; ++i) {
        if(arr[i] != nullptr) {
            delete arr[i];
            arr[i] = nullptr;
        }
    }
    cout << "All Planets has been deleted." << endl;
}
//The function that deletes arrays has been modified to accept a pointer array of Planet objects as an argument, and it deletes all the elements within the array.

void DisplaySpaceshipInventory(vector<Spaceship*>& spaceships) {
    for(int i = 0; i < spaceships.size(); ++i){
        if(spaceships.at(i) != nullptr){
            cout << "Spaceship " << i+1 << ":" << endl;
            spaceships.at(i)->PrintInfo();
            cout << endl;
        }
    }
}
// utilizes the PrintInfo() method from the Spaceship class to display the details of each spaceship stored in the spaceship* vector.

void DisplayMenu() {
    cout << "\nMain Menu:\n";
    cout << "1. Check your funds\n";
    cout << "2. Take a peek at your spacecraft stash\n";
    cout << "3. Discover information about a planet\n";
    cout << "4. Expand your fleet by buying a new spaceship\n";
    cout << "5. Embark on an interplanetary journey\n";
    cout << "6. Embark on quests on a planet\n";
    cout << "7. Save game\n";
    cout << "8. Exit game\n";
    cout << "Please enter your desired option and let the adventure continue! ";
}


void DoEvent(Spaceship** ship, int& balance, vector<Spaceship*>& arr){
    srand(time(NULL));
    int x, y, z;
    std::cout << "\nPreparing for mission on planet " << (*ship)->getPlanet()->getName() << "..." << std::endl;
    std::cout << "Assessing planetary danger level: " << (*ship)->getPlanet()->GetHostility() << std::endl;
    switch((*ship)->getPlanet()->eventleft){ // Verifies the remaining number of occurrences happening on the planet
        case 0:
            cout << "The planet has been scoured and all quests have been completed, leaving no further tasks to undertake.\n" << endl;
            return;
            break; // if there are no more events left on the planet
        
        case 1: // event 1 is the Goldenchest event, it has different levels of based on the hostility.
                // The outcome of victory or defeat determines the acquisition or depletion of funds and resources for the journey.
            ((*ship)->getPlanet()->eventleft)--;
            switch((*ship)->getPlanet()->GetHostility()){
                case 1:
                    cout << "You have found a giant golden loot chest floating in space, filled with precious treasures!" << endl;
cout << "However, a rival spaceship has just arrived and wants to claim the chest for themselves" << endl;
cout << "Their captain challenges you to a battle to determine the chest's rightful owner" << endl;
cout << "Captain: Let's settle this with a game of chance. Pick a number between 0-100, and the closest one to a generated value wins the chest"<< endl;
cout << "Get ready to battle, pilot! Enter your spaceship's number: ";
                    cin >> x;
                    y = rand() % 100;
                    z = rand() % 100;
                    if(abs(x-y) > abs(z-y)){
                        cout << "You lost!" << endl;
                        cout << "You have been defeated in the battle. The enemy forces have plundered 100000 units of fuel and 100000 coins from your ship's reserves";
                        balance -= 100000;
                        if(!((*ship)->UseFuel(100000))){
                            delete *ship;
                            *ship = nullptr;
                            for(int i = 0; i < arr.size(); ++i){
                                if(arr.at(i) == nullptr){
                                    cout << "Your spaceship has depleted all its fuel, rendering it inoperable and beyond recovery." << endl;
                                    switchSpaceships(arr, i, arr.size() - 1);
                                    arr.pop_back();
                                    NumberSpaceships--;
                                    return;
                                }
                            }
                        } 
                    }
                    else{
                        cout << "You won!" << endl;
                        cout << "Congratulations, you have emerged victorious in the battle! You have successfully defeated the rival spaceship and claimed the giant golden loot chest for yourself. Your victory will be remembered in the annals of space history.\n";
                        cout << "Congratulations, you have been rewarded with a fortune of 100,000 units of fuel and 100,000 coins!\n\n";
                        balance += 100000;
                        (*ship)->fueling(100000);
                        return;
                    }
                    break;
                case 2:
                   cout << "You have found a giant golden loot chest floating in space, filled with precious treasures!" << endl;
cout << "However, a rival spaceship has just arrived and wants to claim the chest for themselves" << endl;
cout << "Their captain challenges you to a battle to determine the chest's rightful owner" << endl;
cout << "Captain: Let's settle this with a game of chance. Pick a number between 0-100, and the closest one to a generated value wins the chest"<< endl;
cout << "Get ready to battle, pilot! Enter your spaceship's number: ";
                    cin >> x;
                    y = rand() % 100;
                    if(abs(x-y) > 30){
                        cout << "You lost!" << endl;
                        cout << "You have been defeated in the battle. The enemy forces have plundered 200000 units of fuel and 200000 coins from your ship's reserves";
                        balance -= 200000;
                        if(!((*ship)->UseFuel(200000))){
                            delete *ship;
                            *ship = nullptr;
                            for(int i = 0; i < arr.size(); ++i){
                                if(arr.at(i) == nullptr){
                                    cout << "Your spaceship has depleted all its fuel, rendering it inoperable and beyond recovery." << endl;
                                    switchSpaceships(arr, i, arr.size() - 1);
                                    arr.pop_back();
                                    NumberSpaceships--;
                                    return;
                                }
                            }
                        } 
                    }
                    else{
                        cout << "You won!" << endl;
                        cout << "Congratulations, you have emerged victorious in the battle! You have successfully defeated the rival spaceship and claimed the giant golden loot chest for yourself. Your victory will be remembered in the annals of space history.\n";
                        cout << "Congratulations, you have been rewarded with a fortune of 200000 units of fuel and 200000 coins!\n\n";
                        balance += 200000;
                        (*ship)->fueling(200000);
                        return;
                    }
                    break;
                case 3:
                   cout << "You have found a giant golden loot chest floating in space, filled with precious treasures!" << endl;
cout << "However, a rival spaceship has just arrived and wants to claim the chest for themselves" << endl;
cout << "Their captain challenges you to a battle to determine the chest's rightful owner" << endl;
cout << "Captain: Let's settle this with a game of chance. Pick a number between 0-100, and the closest one to a generated value wins the chest"<< endl;
cout << "Get ready to battle, pilot! Enter your spaceship's number: ";
                    cin >> x;
                    y = rand() % 100;
                    if(abs(x-y) > 10){
                        cout << "You lost!" << endl;
                        cout << "You have been defeated in the battle. The enemy forces have plundered 400000 units of fuel and 400000 coins from your ship's reserves";
                        balance -= 400000;
                        if(!((*ship)->UseFuel(400000))){
                            delete *ship;
                            *ship = nullptr;
                            for(int i = 0; i < arr.size(); ++i){
                                if(arr.at(i) == nullptr){
                                    cout << "Your spaceship has depleted all its fuel, rendering it inoperable and beyond recovery." << endl;
                                    switchSpaceships(arr, i, arr.size() - 1);
                                    arr.pop_back();
                                    NumberSpaceships--;
                                    return;
                                }
                            }
                        }
                    }
                    else{
                        cout << "You won!" << endl;
                        cout << "Congratulations, you have emerged victorious in the battle! You have successfully defeated the rival spaceship and claimed the giant golden loot chest for yourself. Your victory will be remembered in the annals of space history.\n";
                        cout << "Congratulations, you have been rewarded with a fortune of 400000 units of fuel and 400000 coins!\n\n";
                        balance += 400000;
                        (*ship)->fueling(400000);
                        return;
                    }
                    break;
            }
            break;

        case 2: // 2nd event is the raiders event, also based off the hostility of the planet
                // The outcome of victory or defeat determines the acquisition or depletion of funds and resources for the journey.
            ((*ship)->getPlanet()->eventleft)--;
            switch((*ship)->getPlanet()->GetHostility()){
                case 1:
                    cout << "Your ship has been ambushed by a notorious gang of space raiders!\n";
cout << "Your team has taken out most of the raiders, but there's one left: their leader.\n";
cout << "If you don't eliminate him, the raiders will regroup and come back even stronger.\n";
cout << "He's holed up in one of two possible hideouts, and you only have one missile left!\n";
cout << "Select the hideout where you believe he's hiding (enter 1 or 2): ";
                    cin >> x;
                    y = (rand() % 2) + 1;
                    if(x == y){
                        cout << "\n\nYou have killed the Leader of the raiders!\n";
                        cout << "On his body was 100000 coins and you took his remaining crew of 20 members" << endl;
                        balance += 100000;
                        (*ship)->embark(20);
                    }
                    else{
                        cout << "\n\n You have not killed the Leader with your last bomb\n";
                        cout << "Since you are out of weapons, the Leader has taken 100000 coins from you\n";
                        cout << "20 of your crew members have lost faith in you and left you for the raiders" << endl;
                        balance -= 100000;
                        if(!((*ship)->disembark(20))){
                            cout << "Your ship ran out of crew members and you lost the ship" << endl;
                            delete *ship;
                            *ship = nullptr;
                            for(int i = 0; i < arr.size(); ++i){
                                if(arr.at(i) == nullptr){
                                    cout << "AAA" << endl;
                                    switchSpaceships(arr, i, arr.size() - 1);
                                    arr.pop_back();
                                    NumberSpaceships--;
                                    return;
                                }
                            }
                        }
                    }
                    break;
                case 2:
                    cout << "Your ship has been ambushed by a notorious gang of space raiders!\n";
cout << "Your team has taken out most of the raiders, but there's one left: their leader.\n";
cout << "If you don't eliminate him, the raiders will regroup and come back even stronger.\n";
cout << "He's holed up in one of three possible hideouts, and you only have one missile left!\n";
cout << "Select the hideout where you believe he's hiding (enter 1 or 2 or 3): ";
                    cin >> x;
                    y = (rand() % 3) + 1;
                    if(x == y){
                        cout << "\n\nYou have killed the Leader of the raiders!\n";
                        cout << "On his body was 200000 coins and you took his remaining crew of 30 members" << endl;
                        balance += 200000;
                        (*ship)->embark(30);
                    }
                    else{
                        cout << "\n\n You have not killed the Leader with your last bomb\n";
                        cout << "Since you are out of weapons, the Leader has taken 200000 coins from you\n";
                        cout << "30 of your crew members have lost faith in you and left you for the raiders" << endl;
                        balance -= 200000;
                        if(!((*ship)->disembark(30))){
                            cout << "Your ship ran out of crew members and you lost the ship" << endl;
                            delete *ship;
                            *ship = nullptr;
                            for(int i = 0; i < arr.size(); ++i){
                                if(arr.at(i) == nullptr){
                                    cout << "AAA" << endl;
                                    switchSpaceships(arr, i, arr.size() - 1);
                                    arr.pop_back();
                                    NumberSpaceships--;
                                    return;
                                }
                            }
                        }
                    }

                    break;
                case 3:
                    cout << "Your ship has been ambushed by a notorious gang of space raiders!\n";
cout << "Your team has taken out most of the raiders, but there's one left: their leader.\n";
cout << "If you don't eliminate him, the raiders will regroup and come back even stronger.\n";
cout << "He's holed up in one of five possible hideouts, and you only have one missile left!\n";
cout << "Select the hideout where you believe he's hiding (enter 1 or 2 or 3 or 4 or 5): ";
                    cin >> x;
                    y = (rand() % 5) + 1;
                    if(x == y){
                        cout << "\n\nYou have killed the Leader of the raiders!\n";
                        cout << "On his body was 300000 coins and you took his remaining crew of 50 members" << endl;
                        balance += 300000;
                        (*ship)->embark(50);
                    }
                    else{
                        cout << "\n\n You have not killed the Leader with your last bomb\n";
                        cout << "Since you are out of weapons, the Leader has taken 300000 coins from you\n";
                        cout << "50 of your crew members have lost faith in you and left you for the raiders" << endl;
                        balance -= 300000;
                        if(!((*ship)->disembark(50))){
                            cout << "Your ship ran out of crew members and you lost the ship" << endl;
                            delete *ship;
                            *ship = nullptr;
                            for(int i = 0; i < arr.size(); ++i){
                                if(arr.at(i) == nullptr){
                                    cout << "AAA" << endl;
                                    switchSpaceships(arr, i, arr.size() - 1);
                                    arr.pop_back();
                                    NumberSpaceships--;
                                    return;
                                }
                            }
                        }
                    }
                    break;
            }
            break;
    }
    
}

int main(){
  int x;
    // Declare an array called Planetarr to store pointers to Planet objects
Planet* Planetarr[planetCount];

// Populate the array with Planet objects by calling the GeneratePlanets function
GeneratePlanets(Planetarr);
  // Declare a vector called Spacearr to store pointers to objects of the Spaceship class or its subclasses
  

    vector<Spaceship*> Spacearr; // vector of pointers to the subclasses of spaceships. Uses polymorphism to contain all the different types of spaceships in one array.
    // Declare an integer variable to store the player's balance
int playerBalance = 0;
  // Prompt the player to choose between continuing their previous game, starting a new game, or exiting the program
  
    cout << "UNIVERSAL PILGRIMAGE\n";
    cout << "Do you want to resume your previous game progress or initiate a new game session? \n (1) Continue\n (2) New Game\n (3) Exit\nThe decision is yours to make. (1/2/3): ";
    int playerChoice;
  cin >> playerChoice;
    
    if(playerChoice == 1){
        cout << "Retrieving previous game progress...\n";
        LoadGame(Spacearr, Planetarr, playerBalance);
        cout << "Old game loaded" << endl;
    } // loads old game

    else if(playerChoice == 2){
        cout << "Beginning a new game!" << endl;
        playerBalance = 150000; // starting balance
        Spaceship* temp = new NovaCruiser(); // starter spaceship
        NumberSpaceships++;
        Spacearr.push_back(temp);
        Spacearr.at(0)->setPlanet(Planetarr[19]); // starting planet
    } // starts new game

    else if(playerChoice == 3){
        cout << "Shutting Down..." << endl;
        DeleteSpaceships(Planetarr);
        exit(0);
    } // exits program

    else{
        cout << "Invalid Response" << endl;
        DeleteSpaceships(Planetarr);
        exit(0);
    } // exits program

    while (true) {
    DisplayMenu();
    int user_choice;
    cin >> user_choice;
    cout << "\n";

    if (user_choice < 1 || user_choice > 8 || cin.fail()) {
        cout << "Invalid Response\n" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        continue;
    }

    switch (user_choice) {
        case 1:
            cout << "Balance: " << playerBalance << endl;
            break;

        case 2:
            DisplaySpaceshipInventory(Spacearr);
            break;

        case 3:
            cout << "Which Spaceship's Planet would you like to see? #";
            cin >> x;

            if (x <= 0 || x > NumberSpaceships) {
                cout << "Invalid Response!" << endl;
            } else {
                Spacearr.at(x - 1)->getPlanet()->PrintInfo();
                cout << '\n';
            }

            break;

        case 4:
            PurchaseSpaceShip(Spacearr, playerBalance);
            break;

        case 5:
            cout << "Which Spaceship would you like to move? #";
            cin >> x;

            if (x <= 0 || x > NumberSpaceships) {
                cout << "Invalid Response!" << endl;
            } else {
                ExplorePlanet(Planetarr, &(Spacearr.at(x - 1)), Spacearr);
            }

            break;

        case 6:
            cout << "Which Spaceship's planet would you like to do the quest on? #";
            cin >> x;

            if (x <= 0 || x > NumberSpaceships) {
                cout << "Invalid Response!" << endl;
            } else {
                DoEvent(&(Spacearr.at(x - 1)), playerBalance, Spacearr);
            }

            break;

        case 7:
            SaveGame(Spacearr, playerBalance);
            cout << "Saved Data!\n";
            DeleteSpaceships(Planetarr);
            DeleteSpaceships(Spacearr);
            cout << "Ending Game" << endl;
            exit(0);
            break;

        case 8:
            cout << "Exiting Game!" << endl;
            DeleteSpaceships(Planetarr);
            DeleteSpaceships(Spacearr);
            exit(0);
            break;
    }

    if (EndGameLost()) {
        cout << "You Lost!" << endl;
        break;
    } else if (VictoryScreen()) {
        cout << "You Won!" << endl;
        break;
    }
}

DeleteSpaceships(Planetarr);
DeleteSpaceships(Spacearr);
return 0;
}
