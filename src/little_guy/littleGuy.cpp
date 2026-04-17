#include "littleGuy.h"

void LittleGuy::guyInit()
{
	happiness = 5;
}

void LittleGuy::guySad()
{
    happiness -= 1;
}

void LittleGuy::guyHappy()
{
    if(happiness < 5){
        happiness += 1;
    }
}

void LittleGuy::renderGuy() {
	
}