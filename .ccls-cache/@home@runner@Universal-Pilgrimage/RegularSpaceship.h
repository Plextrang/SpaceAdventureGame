#ifndef NovaCruiser_H
#define NovaCruiser_H
#include "Spaceship.h"
#include "Planet.h"

class NovaCruiser : public Spaceship{
    public:
        NovaCruiser();
        void PrintInfo() const; // virtual function
        

};

#endif