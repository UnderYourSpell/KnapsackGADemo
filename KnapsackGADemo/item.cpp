#include "item.h"

item::item()
{
	id = 0;
	weight = 0;
	included = 0;
}

item::item(int id, int weight, char included) : id(id), weight(weight), included(included)
{
}

item::~item()
{
}

