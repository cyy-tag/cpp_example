#pragma once
#include <iostream>
#include "animal.h"

class Dog : public Animal
{
public:
    Dog()=default;
    void Sound() override {
        std::cout << " dog sound " << std::endl;
    }
    ~Dog()=default;
};
