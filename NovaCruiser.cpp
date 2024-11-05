#include "NovaCruiser.h"
#include "Planet.h"
#include <iostream>
using namespace std;

NovaCruiser::NovaCruiser() : Spaceship(375000, 75){}; //  maximum values

void NovaCruiser::PrintInfo() const{
    cout << "Spacecraft's name: NovaCruiser" << endl;
    cout << "Current Crew: " << curr_crew << endl;
    cout << "Current fuel: " << curr_fuel << endl;
    cout << "Current Planet: " << currPlanet->getName() << endl;
    cout << "Coordinates: (" << currPlanet->x << ", " << currPlanet->y << ")" << endl;
}