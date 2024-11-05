#ifndef NOVACRUISER_H
#define NOVACRUISER_H

#include "Spaceship.h"
#include "Planet.h"

class NovaCruiser : public Spaceship {
public:
    NovaCruiser();
    void PrintInfo() const; // This is virtual function
};

#endif