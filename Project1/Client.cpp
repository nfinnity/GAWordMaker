// Project1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GAWordMaker.h"
#include "Individual.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void grabInput(string& word, double& mutation, int& populationSize, int& matingMethod, int& crossoverMethod)
{
	cout << "Please enter parameter values." << endl;
	cout << "Mutation rate (in %): ";
	cin >> mutation;
	while (!cin)
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Invalid input. Please try again." << endl;
		cout << "Mutation rate (in %): ";
		cin >> mutation;
	}
	mutation = mutation / 100.0;

	cout << "Population size: ";
	cin >> populationSize;
	while (!cin)
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Invalid input. Please try again." << endl;
		cout << "Population size: ";
		cin >> populationSize;
	}

	cout << "Mating method options: 0 -> Generational, 1 -> Elitist, 2 -> Parent Replacement" << endl;
	cout << "Mating method (enter digit): ";
	cin >> matingMethod;
	while (!cin || (matingMethod > 2) || (matingMethod < 0))
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Invalid input. Please try again." << endl;
		cout << "Mating method (enter digit): ";
		cin >> matingMethod;
	}

	cout << "Crossover method options: 0 -> One-point, 1 -> Two-point" << endl;
	cout << "Crossover method (enter digit): ";
	cin >> crossoverMethod;
	while (!cin || (crossoverMethod > 1) || (crossoverMethod < 0))
	{
		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "Invalid input. Please try again." << endl;
		cout << "Crossover method (enter digit): ";
		cin >> crossoverMethod;
	}
	cout << endl << endl;
}



int main()
{
	string word, defaultCheck;
	double mutation;
	int populationSize, crossoverMethod, matingMethod;
	vector<individual> currPopulation, nextGen;
	int iterationCount = 1;
	
	cout << "WORDMAKER GENETIC ALGORITHM" << endl << endl;
	cout << "Which word would you like to find? ";
	getline(cin, word);
	cout << "Would you like to enter manual parameter values (y/n)? ";
	cin >> defaultCheck;
	if (defaultCheck.compare("y") == 0)
	{
		grabInput(word, mutation, populationSize, matingMethod, crossoverMethod);
	}
	else
	{
		mutation = 0.1;
		populationSize = 300;
		crossoverMethod = 1;
		matingMethod = 1;
	}
	cout << "Word chosen: " << word << endl; 
	cout << "Mutation Rate: " << mutation  << endl;
	cout << "Population Size: " << populationSize << endl;

	if (matingMethod == 0)
		cout << "Mating Operator: Generational" << endl;
	else if (matingMethod == 1)
		cout << "Mating Operator: Elitist" << endl;
	else if (matingMethod == 2)
		cout << "Mating Operator: Parent Replacement" << endl;

	if (crossoverMethod == 0)
		cout << "Selection Operator: One-point" << endl;
	else if (crossoverMethod == 1)
		cout << "Selection Operator: Two-point" << endl;
	
	vector<individual> temp;
	GAWordMaker sample = GAWordMaker(mutation, populationSize, word);
	sample.initializePopulation(currPopulation, nextGen);
	while (iterationCount < 10000)
	{
		sample.calculateFitness(currPopulation);
		sample.sortByFitness(currPopulation);
		switch (matingMethod)
		{
		case 0:
			sample.mateGenerational(currPopulation, nextGen, crossoverMethod);
			temp = currPopulation;
			currPopulation = nextGen;
			break;
		case 1:
			sample.mateGapElitist(currPopulation, nextGen, crossoverMethod);
			temp = currPopulation;
			currPopulation = nextGen;
			break;
		case 2:
			sample.mateGapParentOffSpring(currPopulation, nextGen, crossoverMethod);
			temp = currPopulation;
			currPopulation = nextGen;
			break;
		}
		cout << "Best match: " << currPopulation.at(0).genes << endl;
		if (currPopulation.at(0).fitness == 0)
			break;
		iterationCount++;
	}
	sample.calculateFitness(currPopulation);
	sample.sortByFitness(currPopulation);
	cout << "Solution was found in generation: " << iterationCount << endl;
	cout << "Solution word: " << currPopulation.at(0).genes << "    Solution fitness: " << currPopulation.at(0).fitness << endl;
}