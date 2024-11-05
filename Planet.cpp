#include "Planet.h"
#include <iostream>
#include <random>
#include <string>
using namespace std;

string arr[20] = {"Aelion", "Boreas", "Hyperion", "Galaxar", "Xenovia", "Orion's Belt", "Novacore", "Arcadia", "Cygnus", "Nibiru", 
                        "Asterion", "Phoenicia", "Vespera", "Celestia", "Xanthe", "Krynn", "Andromeda", "Tauris", "Orionis", "Vega"}; 

Planet::Planet(int diff, int num) {
    srand(time(NULL));
    x = rand() % 255 + 1;
    y = rand() % 255 + 1;
    hostility = diff;
    name = arr[num];
} // This generates random x and y coordinates for the planet.

Planet::~Planet() {
    cout << "Planet " << name << " has been destroyed." << endl;
} //destroys planet

void Planet::PrintInfo() const {
  // Output planet information
    cout << "Information for Planet " << name << ":" << endl;
    cout << "- Difficulty: " << hostility << endl;
    cout << "- Coordinates: (" << x << ", " << y << ")" << endl;
}


int Planet::GetHostility(){
    return hostility;
}

string Planet::getName(){
    return name;
}