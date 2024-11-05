#ifndef INFINITYFUELER_H
#define INFINITYFUELER_H

#include "Spaceship.h"
#include "Planet.h"

class InfinityFueler : public Spaceship{
    public:
        InfinityFueler();
        void PrintInfo() const; //this is virtual function
    
};

#endif