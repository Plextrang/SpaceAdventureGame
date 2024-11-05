#ifndef SPACESHIP_H
#define SPACESHIP_H
#include "Planet.h"
#include <string>

using namespace std;

class Spaceship{
    protected:
        Planet* currPlanet; // pointer that refers to the object of the planet that the spaceship is currently on.
        int fuel_capacity, crew_capacity; // The amount of fuel and crew that the spaceship can carry is predetermined and unchangeable.
        
    
    public:
        string getName(void);
        int curr_fuel, curr_crew; 
        bool disembark(int); // --crew
        int getcrewcap(void);
        void embark(int); // ++crew 
        Spaceship(int, int); // constructor
        void fueling(int); // ++fuel
        virtual ~Spaceship(void); // virtual destructor
        bool UseFuel(int); // --fuel
        void setPlanet(Planet*); 
        Planet* getPlanet(void);
        virtual void PrintInfo(void) const = 0; // This will designate the class as an abstract class.
        int getfuelcap(void);
};

#endif