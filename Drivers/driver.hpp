#pragma once

#include "types.hpp"

struct Driver
{    
    Driver() = default;
    ~Driver() = default;

    virtual void    activate();
    virtual int32_t reset();
    virtual void    deactivate();
};

class DriverManager final
{
private:
    int8_t num_drivers;

    Driver* drivers[255];

public:
    DriverManager();
    ~DriverManager() = default;

    void add_new_driver(Driver* driver);
    void activae_all();
};
