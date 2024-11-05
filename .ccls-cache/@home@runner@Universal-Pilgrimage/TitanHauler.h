#ifndef TitanHauler_H
#define TitanHauler_H

#include "Spaceship.h"
#include "Planet.h"

class TitanHauler : public Spaceship{
    public:
        TitanHauler() : Spaceship(250000, 100){} // const 
        void PrintInfo() const; // virtual function
};
#endif