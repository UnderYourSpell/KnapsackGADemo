#pragma once
#include <iostream>
#include <string>
using namespace std;
class Genome
{
public:
	Genome();
	Genome(string gene);
	~Genome();
	string getGene() {
		return gene;
	}

	void setWeight(int newWeight) {
		weight = newWeight;
	}

	void setGene(string newGene) {
		gene = newGene;
	}

	int getWeight() {
		return weight;
	}

private:
	string gene;
	int weight;
};

