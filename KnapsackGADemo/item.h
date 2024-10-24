#pragma once
#include <string>
using namespace std;
class item
{
public:
	item();
	item(int id, int weight, char included);
	~item();
	int getID() {
		return id;
	}
	int getWeight() {
		return weight;
	}

private:
	int id;
	int weight;
	char included;
};

