#ifndef REGISTRY_H
#define REGISTRY_H
#pragma once

#include "Map.h"
#include "Person.h"

template<typename T>
using ulist = std::vector<std::unique_ptr<T>>;

class registry  
{
private:


public:

	ulist<Map> maps_;
	ulist<person> people_;
	registry() : maps_{}, people_{} {

	}

};
#endif