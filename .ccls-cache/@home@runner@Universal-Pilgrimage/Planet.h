#ifndef PLANET_H
#define PLANET_H

#include <ctime>
#include <string>

using namespace std;

class Planet {
    private:
        int hostility; // hostility on planet level
        string name; // planet name
    
    public:
        int eventleft = 2; 
        int x, y; 
        Planet(int diff, int num); // constructor
        int GetHostility(); // returns the level of hostility on the planet
        string getName(); // returns the name of the planet
        ~Planet(); // destructor
        void PrintInfo() const; // prints information about the planet
};

#endif
