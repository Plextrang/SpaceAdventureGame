// Import necessary header files
#include "InfinityFueler.h"
#include "Planet.h"
#include <iostream>
using namespace std;

// Define the class constructor with fixed stats
InfinityFueler::InfinityFueler() : Spaceship(500000, 50){}

// Define a function to print the spaceship's information
void InfinityFueler::PrintInfo() const {
// Print the spaceship's type, crew, fuel, current planet name, and coordinates
cout << "Spacecraft's Name: InfinityFueler" << endl;
cout << "Current Crew: " << curr_crew << endl;
cout << "Current Fuel: " << curr_fuel << endl;
cout << "Current Planet: " << currPlanet->getName() << endl;
cout << "Coordinates: (" << currPlanet->x << ", " << currPlanet->y << ")" << endl;
}