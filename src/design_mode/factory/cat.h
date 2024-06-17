#pragma once
#include <iostream>
#include "animal.h"

class Cat : public Animal
{
public:
    Cat()=default;
    void Sound() override {
        std::cout << " cat sound " << std::endl;
    }
    ~Cat()=default;
};
