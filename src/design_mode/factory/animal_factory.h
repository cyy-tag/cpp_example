#pragma once
#include <memory>
#include "cat.h"
#include "dog.h"

enum class AnimalType {
    Dog = 1,
    Cat = 2
};

class AnimalFactory
{
public:
    std::unique_ptr<Animal> CreateAnimal(AnimalType type) {
        switch (type)
        {
        case AnimalType::Dog:
            return std::make_unique<Dog>();
        case AnimalType::Cat:
            return std::make_unique<Cat>();
        default:
            return nullptr;
        }
    }
};
