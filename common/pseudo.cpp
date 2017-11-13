
#include "pseudo.h"

#include <thread>
#include <chrono>
#include <random>
#include <iostream>

//--------------------------

using namespace std;

int random_sleep(char c, unsigned int times) 
{
    std::default_random_engine dre(c);
    std::uniform_int_distribution<int> id(10, 1000);
        
    for (unsigned int i = 0; i < times; i++) {
	this_thread::sleep_for(chrono::milliseconds(id(dre)));
	cout.put(c).flush();
    }
    
    return c;
}
