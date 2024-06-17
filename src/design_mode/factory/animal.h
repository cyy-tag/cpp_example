#pragma once

class Animal
{
public:
    Animal()=default;
    virtual void Sound()=0;
    virtual ~Animal()=default;
};
