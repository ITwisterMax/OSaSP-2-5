#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <functional>

#include "Api/CustomQueue.h"
#include "Api/CustomHandler.h"

using namespace std;

// Default constant values
constexpr auto PATH = "D:\\Test.txt";
constexpr auto THREADS_COUNT = 5;

// Vectors for input and split strings
vector<string>* inputStrings = new vector<string>();
vector<vector<string>*>* splitStrings = new vector<vector<string>*>();

bool getDataFromFile(string path, vector<string>* inputStrings)
{
	ifstream file(path);

	// Check file stream
	if (file.good()) 
	{
		string currentLine;

		// Get lines and push it in input strings
		while (getline(file, currentLine))
		{
			if (currentLine.size() != 0)
			{
				inputStrings->push_back(currentLine);
			}
		}

		return true;
	}
	
	return false;
}

void print(vector<string> outputString)
{
	// Print data feom vectors
	for (size_t i = 0; i < outputString.size(); i++)
	{
		cout << outputString[i] << endl;
	}

	cout << endl;
}

void splitData(CustomQueue customQueue, int threadCount)
{
	// Calculate lines count in one thread
	size_t linesCount = (size_t)floor(inputStrings->size() / threadCount);

	for (size_t i = 0; i < inputStrings->size(); i += linesCount)
	{
		// Create new vector and push it in split strings vectors
		vector<string>* newVector = new vector<string>();
		splitStrings->push_back(newVector);

		// Get data from lines and push it in vectors
		for (size_t j = i; j < i + linesCount; j++)
		{
			if (j < inputStrings->size())
			{
				string str = (*inputStrings)[j];
				newVector->push_back(str);
			}
		}

		// Sort strings
		customQueue.push([newVector]() {
			std::vector<std::string>* copy = newVector;
			std::sort(newVector->begin(), newVector->end());
		});
	}
}

vector<string> mergeTwoStrings(vector<string> firstString, vector<string> secondString)
{
	size_t firstVectorSize = firstString.size();
	size_t secondVectorSize = secondString.size();

	vector<string> outputString;
	outputString.reserve(firstVectorSize + secondVectorSize);

	size_t i = 0;
	size_t j = 0;

	// Compare chars and push it in necessary order
	while (i < firstVectorSize && j < secondVectorSize)
	{
		if (firstString[i] <= secondString[j])
		{
			outputString.push_back(firstString[i++]);
		}
		else
		{
			outputString.push_back(secondString[j++]);
		}
	}

	// Case when strings has different length
	while (i < firstVectorSize)
	{
		outputString.push_back(firstString[i++]);
	}

	while (j < secondVectorSize) {
		outputString.push_back(secondString[j++]);
	}

	return outputString;
}

vector<string> mergeStrings()
{
	vector<string> prevString;

	// Get first string
	if (splitStrings->size() > 0)
	{
		prevString = *(*splitStrings)[0];
	}
		
	// Merge it with next string
	for (size_t i = 1; i < splitStrings->size(); i++)
	{
		prevString = mergeTwoStrings(prevString, *(*splitStrings)[i]);
	}

	return prevString;
}

int main()
{
	// Try to get data from file
	if (getDataFromFile(PATH, inputStrings))
	{
		CustomQueue taskQueue;
		CustomHandler taskExecutor(taskQueue);

		// Print input vectors
		cout << "Input strings:" << endl << endl;
		print(*inputStrings);

		// Calculate threadCount
		int threadsCount = THREADS_COUNT > inputStrings->size() ? inputStrings->size() : THREADS_COUNT;

		// Split input vectors and sort them
		splitData(taskQueue, threadsCount);
		taskExecutor.execute(threadsCount);

		// Create outpur vectors
		vector<string> sortedVectors = mergeStrings();

		// Print output vectors
		cout << "Output strings:" << endl << endl;
		print(sortedVectors);

		return 0;
	}

	return -1;	
}
