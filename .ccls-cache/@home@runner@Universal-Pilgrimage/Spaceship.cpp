#include "Spaceship.h"
#include "Planet.h"
#include <iostream>

using namespace std;

// Constructor for Spaceship class
Spaceship::Spaceship(int fuel, int crew){
    fuel_capacity = fuel;
    crew_capacity = crew;
    curr_crew = crew;
    curr_fuel = fuel;
}

// Add crew to the ship
void Spaceship::embark(int x){
    // Check if adding x crew members would exceed the capacity of the ship
    // If it does, set the current number of crew members to the maximum capacity
    // Otherwise, add x to the current number of crew members
    ((curr_crew + x) > crew_capacity) ? (curr_crew = crew_capacity) : (curr_crew += x);
}

// Remove crew from the ship
bool Spaceship::disembark(int x){
    // Check if removing x crew members would result in negative crew count
    // If it does, set the current number of crew members to 0
    // Otherwise, subtract x from the current number of crew members
    ((curr_crew - x) > 0) ? (curr_crew -= x) : (curr_crew = 0);
    // Return true if there are crew members left, false otherwise
    return !(curr_crew == 0);
}

// Add fuel to the ship
void Spaceship::fueling(int x){
    // Check if adding x fuel would exceed the fuel capacity of the ship
    // If it does, set the current fuel level to the maximum capacity
    // Otherwise, add x to the current fuel level
    ((curr_fuel + x) > fuel_capacity) ? (curr_fuel = fuel_capacity) : (curr_fuel += x);
}

// Remove fuel from the ship
bool Spaceship::UseFuel(int num){
    // Check if removing num fuel would result in negative fuel level
    // If it does, set the current fuel level to 0
    // Otherwise, subtract num from the current fuel level
    if((curr_fuel - num) < 0){
        curr_fuel = 0;
        return false;
    }
    curr_fuel -= num;
    return true;
}

// Destructor for Spaceship class
Spaceship::~Spaceship(){
    cout << "Deleted Ship" << endl;
}

// Set the current planet of the ship
void Spaceship::setPlanet(Planet* planet){
    currPlanet = planet;
}

// Get the current planet of the ship
Planet* Spaceship::getPlanet(){
    return currPlanet;
}

// Get the maximum crew capacity of the ship
int Spaceship::getcrewcap(){
    return crew_capacity;
}

// Get the maximum fuel capacity of the ship
int Spaceship::getfuelcap(){
    return fuel_capacity;
}
