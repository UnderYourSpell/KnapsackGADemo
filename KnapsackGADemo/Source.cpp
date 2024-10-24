#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>
#include "item.h"
#include "Genome.h"
using namespace std;

const int WEIGHT = 1000;
const int POSSIBLE_ITEMS = 100;
const int MAX_WEIGHT = 100;
const int POP_SIZE = 24;
const int MAX_GENERATIONS = 100;
const int ELITISM = 2;
const int PROB_MUTATION = 2;

const int TARGET = 5000; //target weight for knapsack

string createRandomGenome() {
	string gene;
	for (int i = 0; i <= POSSIBLE_ITEMS; i++) {
		int randNum = rand() % 2; //either 0 or 1
		if (randNum == 0) {
			gene = gene + "0";
		}
		else {
			gene = gene + "1";
		}
	}
	return gene;
}

bool compareWeights(Genome i1, Genome i2) {
	return(i1.getWeight() > i2.getWeight());
}

void fitness(vector<Genome> &genePool, vector<item*> itemList) {
	for (auto i = genePool.begin(); i != genePool.end(); ++i) {
		string sequence = i->getGene();
		int weight = 0;
		int iterator = 0;
		for (auto j = itemList.begin(); j != itemList.end(); j++) {
			if (sequence[iterator] == '1') {
				weight += (*j)->getWeight();
			}
			iterator++;
		}
		if (weight > TARGET) {
			weight = 0;
		}
		i->setWeight(weight);
	}
}

void spliceGenes(Genome &gene1, Genome &gene2) {
	//single point crossover function
	int splicePoint = rand() % POSSIBLE_ITEMS;
	string seq1 = gene1.getGene();
	string seq2 = gene2.getGene();

	string seq1BackHalf = seq1.substr(splicePoint, seq1.length() - splicePoint);
	string seq1FrontHalf = seq1.substr(0, splicePoint);
	string seq2BackHalf = seq2.substr(splicePoint, seq2.length() - splicePoint);
	string seq2FrontHalf = seq2.substr(0, splicePoint);

	string newSeq1 = seq1FrontHalf + seq2BackHalf;
	string newSeq2 = seq2FrontHalf + seq1BackHalf;

	gene1.setGene(newSeq1);
	gene2.setGene(newSeq2);
}

void mutate(Genome& gene) { //1% chance of mutation
	int threshold = POSSIBLE_ITEMS / POSSIBLE_ITEMS; 
	string sequence = gene.getGene();
	for (int i = 0; i <= POSSIBLE_ITEMS; i++) {
		int randMutate = rand() % POSSIBLE_ITEMS;
		if (randMutate < threshold) {
			if (sequence[i] == '1') {
				sequence[i] = '0';
			}
			else {
				sequence[i] = '1';
			}
		}
	}
	gene.setGene(sequence);
}

int main() {
	srand(time(NULL));
	vector<item*> itemList;
	vector<Genome> genePool;

	//setting random weights for each item
	for (int i = 0; i <= POSSIBLE_ITEMS; i++) {
		item* tempItem = new item(i, rand() % MAX_WEIGHT + 1, 0);
		itemList.push_back(tempItem);
	}
	
	//create initial gene sequences
	for (int i = 0; i <= POP_SIZE; i++) {
		string sequence = createRandomGenome();
		Genome newGene = Genome(sequence);
		genePool.push_back(newGene);
	}

	//from here is where we will loop until we have the best generation
	for (int p = 0; p <= MAX_GENERATIONS; p++) {
		fitness(genePool, itemList); //determines fitness of gene pool
		sort(genePool.begin(), genePool.end(), compareWeights); //sorts the gene pool

		vector<Genome> newGen; //new generation

		//populate new Generation with top 2 - these are the elite solutions
		for (auto i = genePool.begin(); i != genePool.begin() + ELITISM; ++i) {
			newGen.push_back(*i);
		}

		std::vector<Genome> splicedGenes;
		//not really selecting optimal parents
		// we are creating too many synthetic genomes
		for (auto i = genePool.begin() + ELITISM; i != genePool.end() - (POP_SIZE / 2) - 1; ++i) {
			Genome gene1 = *i;
			if (i + 1 != genePool.end()) {
				Genome gene2 = *(i + 1);
				spliceGenes(gene1, gene2);
				splicedGenes.push_back(gene1);
				splicedGenes.push_back(gene2);
			}
			else {
				break;
			}
		}

		// doing the fitness for the new genes
		fitness(splicedGenes, itemList);

		//add new genes to genePool
		genePool.insert(genePool.end(), splicedGenes.begin(), splicedGenes.end());

		//re-sort
		sort(genePool.begin(), genePool.end(), compareWeights);
		
		//populate new Generation for only pop size
		for (auto i = genePool.begin()+ELITISM; i != genePool.begin() + POP_SIZE; ++i) {
			newGen.push_back(*i);
		}

		//now we have a generation - but before that, we shall mutate
		for (auto i = newGen.begin(); i != newGen.end(); ++i) {
			mutate(*i);
		}

		//fitness after mutation
		fitness(newGen, itemList);
	
		genePool.clear();
		genePool = newGen; //re setting the genePool to newGen
		newGen.clear();
	}

	sort(genePool.begin(), genePool.end(), compareWeights);
	cout << "Final Generation" << endl;
	for (auto i = genePool.begin(); i != genePool.begin() + POP_SIZE; ++i) {
		cout << i->getWeight() << endl;
	}
	
	return 0;
}