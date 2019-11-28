#include <RushVersion.h>
#include <print.h>

#include <iostream>

void print(){
    std::cout << "Rush Version: " << RUSH_VERSION_MAJOR << "." << RUSH_VERSION_MINOR << std::endl;
    std::cout << "Rush Platform: " << PLATFORM << std::endl;
}