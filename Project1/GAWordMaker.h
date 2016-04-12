#ifndef GAWORDMAKER_H
#define GAWORDMAKER_H

#include "stdafx.h"
#include "Individual.h"
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class GAWordMaker
{
	int m_populationSize;
	double m_mutation;
	string m_targetString;
public:
	void initializePopulation(vector<individual>&, vector<individual>&);
	GAWordMaker(double, int, string);
	void calculateFitness(vector<individual>&);
	int GAWordMaker::randomNumber(int, int);
	void GAWordMaker::sortByFitness(vector<individual>&);
	static bool GAWordMaker::fitnessSort(const individual, const individual);
	void GAWordMaker::mateGenerational(vector<individual>&, vector<individual>&, int);
	void GAWordMaker::mateGapElitist(vector<individual>&, vector<individual>&, int);
	void GAWordMaker::mateGapParentOffSpring(vector<individual>&, vector<individual>&, int);
	void GAWordMaker::onePointCrossover(int&, int&, individual&, individual&, individual, individual);
	void GAWordMaker::twoPointCrossover(int&, int&, individual&, individual&, individual, individual);
	void GAWordMaker::mutate(vector<individual>&);
};

#endif