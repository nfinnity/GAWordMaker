#include "stdafx.h"
#include "GAWordMaker.h"
#include "Individual.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <math.h>


using namespace std;


GAWordMaker::GAWordMaker(double mutation, int populationSize, string targetString)
{
	m_mutation = mutation;
	m_targetString = targetString;
	m_populationSize = populationSize;
}

void GAWordMaker::initializePopulation(vector<individual>& currPopulation, vector<individual>& nextGen)
{
	
	int geneticLength = m_targetString.length();

	for (int i = 0; i < m_populationSize; i++)
	{
		individual member;
		member.fitness = 0;
		member.genes = "";

		for (int j = 0; j < geneticLength; j++)
		{
			member.genes += randomNumber(32, 122);
		}
		currPopulation.push_back(member);
	}
	nextGen.resize(m_populationSize);
}

void GAWordMaker::calculateFitness(vector<individual>& vector)
{
	int geneticLength = m_targetString.length();
	int fitnessScore = 0;

	for (int i = 0; i < vector.size(); i++)
	{
		for (int j = 0; j < geneticLength; j++)
		{
			fitnessScore += abs(int(m_targetString[j] - vector.at(i).genes[j]));
			if (m_targetString[j] != vector.at(i).genes[j])
				fitnessScore += 5;
		}
		vector.at(i).fitness = fitnessScore;
		fitnessScore = 0;
	}
}

int GAWordMaker::randomNumber(int lower, int upper)
{
	random_device generator;
	mt19937 gen(generator());
	uniform_int_distribution<int> dis(lower, upper);
	int value = dis(gen);
	return value;
}

void GAWordMaker::mateGenerational(vector<individual>& currPopulation, vector<individual>& nextGen, int crossoverMethod)
{
	individual offspring1, offspring2;
	int parent1, parent2;
	nextGen.clear();
	while (nextGen.size() < m_populationSize)
	{
		int parent1, parent2;
		bool sameParent = false;
		do
		{
			parent1 = randomNumber(0, m_populationSize - 1);
			parent2 = randomNumber(0, m_populationSize - 1);
			if (parent1 == parent2)
				sameParent = true;
			else
				sameParent = false;
		} while (sameParent);

		int firstPoint, secondPoint;
		if (crossoverMethod == 0)
			onePointCrossover(firstPoint, secondPoint, offspring1, offspring2, currPopulation.at(parent1), currPopulation.at(parent2));
		else if (crossoverMethod == 1)
			twoPointCrossover(firstPoint, secondPoint, offspring1, offspring2, currPopulation.at(parent1), currPopulation.at(parent2));

		if (randomNumber(0, 1) < 0.5)
			nextGen.push_back(offspring1);
		else
			nextGen.push_back(offspring2);

		if (randomNumber(0, 1) < m_mutation)
			mutate(nextGen);
	}
}

void GAWordMaker::mateGapElitist(vector<individual>& currPopulation, vector<individual>& nextGen, int crossoverMethod)
{
	individual offspring1, offspring2;
	int elitistGroup = m_populationSize*0.1f;
	nextGen.clear();
	for (int i = 0; i < elitistGroup; i++)
	{
		nextGen.push_back(currPopulation.at(i));
	}
	while (nextGen.size() < m_populationSize)
	{
		int parent1, parent2;
		bool sameParent = false;
		do
		{
			parent1 = randomNumber(0, m_populationSize - 1);
			parent2 = randomNumber(0, m_populationSize - 1);
			if (parent1 == parent2)
				sameParent = true;
			else
				sameParent = false;
		} while (sameParent);

		int firstPoint, secondPoint;
		if (crossoverMethod == 0)
			onePointCrossover(firstPoint, secondPoint, offspring1, offspring2, currPopulation.at(parent1), currPopulation.at(parent2));
		else if (crossoverMethod == 1)
			twoPointCrossover(firstPoint, secondPoint, offspring1, offspring2, currPopulation.at(parent1), currPopulation.at(parent2));

		if (randomNumber(0,1) < 0.5)
			nextGen.push_back(offspring1);
		else 
			nextGen.push_back(offspring2);
		if (randomNumber(0, 1) < m_mutation)
			mutate(nextGen);
	}
}

void GAWordMaker::mateGapParentOffSpring(vector<individual>& currPopulation, vector<individual>& nextGen, int crossoverMethod)
{
	individual offspring1, offspring2;
	vector<individual> selectorByFitness;
	nextGen.clear();
	int parent1, parent2;

	while (nextGen.size() < m_populationSize)
	{
		int parent1, parent2;
		bool sameParent = false;
		do
		{
			parent1 = randomNumber(0, m_populationSize - 1);
			parent2 = randomNumber(0, m_populationSize - 1);
			if (parent1 == parent2)
				sameParent = true;
			else
				sameParent = false;
		} while (sameParent);

		int firstPoint, secondPoint;
		if (crossoverMethod == 0)
			onePointCrossover(firstPoint, secondPoint, offspring1, offspring2, currPopulation.at(parent1), currPopulation.at(parent2));
		else if (crossoverMethod == 1)
			twoPointCrossover(firstPoint, secondPoint, offspring1, offspring2, currPopulation.at(parent1), currPopulation.at(parent2));

		selectorByFitness.push_back(offspring1);
		selectorByFitness.push_back(offspring2);
		selectorByFitness.push_back(currPopulation.at(parent1));
		selectorByFitness.push_back(currPopulation.at(parent2));
		calculateFitness(selectorByFitness);
		sort(selectorByFitness.begin(), selectorByFitness.end(), fitnessSort);
		nextGen.push_back(selectorByFitness.at(0));

		if (randomNumber(0, 1) < m_mutation)
			mutate(nextGen);
		
		selectorByFitness.clear();
	}
}

bool GAWordMaker::fitnessSort(const individual x, const individual y)
{
	return (x.fitness < y.fitness);
}

void GAWordMaker::sortByFitness(vector<individual>& currPopulation)
{
	sort(currPopulation.begin(), currPopulation.end(), fitnessSort);
}

void GAWordMaker::onePointCrossover(int& firstPoint, int& secondPoint, individual& offspring1, individual& offspring2, individual parent1, individual parent2)
{
	int geneticLength = m_targetString.length();
	firstPoint = randomNumber(0, geneticLength);
	secondPoint = geneticLength;
	offspring1.genes = parent1.genes.substr(0, firstPoint) + parent2.genes.substr(firstPoint, geneticLength-firstPoint);
	offspring2.genes = parent2.genes.substr(0, firstPoint) + parent1.genes.substr(firstPoint, geneticLength-firstPoint);
}

void GAWordMaker::twoPointCrossover(int& firstPoint, int& secondPoint, individual& offspring1, individual& offspring2, individual parent1, individual parent2)
{
	int geneticLength = m_targetString.length();
	firstPoint = randomNumber(0, geneticLength);
	secondPoint = randomNumber(0, geneticLength);
	if (firstPoint > secondPoint)
	{
		int temp = firstPoint;
		firstPoint = secondPoint;
		secondPoint = temp;
	}
	offspring1.genes = parent1.genes.substr(0, firstPoint) + parent2.genes.substr(firstPoint, secondPoint-firstPoint) + parent1.genes.substr(secondPoint);
	offspring2.genes = parent2.genes.substr(0, firstPoint) + parent1.genes.substr(firstPoint, secondPoint - firstPoint) + parent2.genes.substr(secondPoint);
}



void GAWordMaker::mutate(vector<individual>& population)
{
	int stringPos = randomNumber(0, m_targetString.length());
	int newChar = randomNumber(32, 122);
	vector<individual> temp;
	temp.push_back(population.back());
	individual member = temp.back();
	member.genes[stringPos] = newChar;
	population.pop_back();
	population.push_back(member);
}