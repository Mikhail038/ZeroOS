#include "driver.hpp"
#include "types.hpp"

void    Driver::activate()
{

}

int32_t Driver::reset()
{

}

void    Driver::deactivate()
{

}



DriverManager::DriverManager() : 
    num_drivers(0)
{}

void DriverManager::add_new_driver(Driver* driver)
{
    drivers[num_drivers] = driver;
    ++num_drivers;
}

void DriverManager::activae_all()
{
    for (uint8_t cnt = 0; cnt != num_drivers; ++cnt)
    {
        drivers[cnt]->activate();
    }
}
