#include "interactable.hpp"
#include <iostream>

	
interactable::~interactable()
{
	
}

void interactable::interact()
{
    std::cout << "you interacted with me!";
}