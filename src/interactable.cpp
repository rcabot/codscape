#include "interactable.hpp"
#include <iostream>

void interactable::interact()
{
    on_interact_(reg);
}