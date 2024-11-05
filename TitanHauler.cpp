// include necessary header files
#include <iostream>
#include "Planet.h"
#include "TitanHauler.h"

using namespace std;

// method to print the information of the TitanHauler spacecraft
void TitanHauler::PrintInfo() const {
// output the spacecraft's name
cout << "Name of spacecraft: TitanHauler" << endl;
// output the current crew number
cout << "Current crew on board: " << curr_crew << endl;
// output the current fuel level
cout <<"Current fuel level: " << curr_fuel << endl;
// output the current planet name
cout << "Current planet: " << currPlanet->getName() << endl;
// output the planet's coordinates
cout << "Coordinates of planet: (" << currPlanet->x << ", " << currPlanet->y << ")" << endl;
}