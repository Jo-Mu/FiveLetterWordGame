// FiveLetterWordGame.cpp : Defines the entry point for the console application.
//

#define NOMINMAX;
#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <random>
#include <cctype>
#include <algorithm>


std::string GetExePath()
{
	char result[MAX_PATH];
	return std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
}

std::vector<std::string> LoadWords(const std::string fileName)
{
	std::vector<std::string> fiveLetterWords;
	std::string filePath = GetExePath();
	filePath = filePath.substr(0, filePath.size() - 23) + "\\" + fileName;

	std::ifstream wordList(filePath);

	for(std::string line; std::getline(wordList, line);)
	{
		if(line.empty())
		{
			continue;
		}

		fiveLetterWords.push_back(line);
	}

	return fiveLetterWords;
}

bool IsWordInList(const std::vector<std::string>& wordList, const std::string& guess)
{
	for(const std::string word : wordList)
	{
		if(guess == word)
		{
			return true;
		}
	}

	return false;
}

std::vector<int> GetCharBucket(const std::string& word)
{
	std::vector<int> charBucket(26, 0);

	for(const char& c : word)
	{
		charBucket[c - 'a']++;
	}

	return charBucket;
}

int MatchScore(const std::string& word, const std::string& guess)
{
	int score = 0;

	const std::vector<int> wordBucket = GetCharBucket(word);
	const std::vector<int> guessBucket = GetCharBucket(guess);

	for(int i = 0; i < 26; i++)
	{
		score += std::min(wordBucket[i], guessBucket[i]);
	}

	for(int i = 0; i < 5; i++)
	{
		if(guess[i] == word[i])
		{
			score++;
		}
	}

	return score;
}

int main()
{
	std::vector<std::string> fiveLetterWords = LoadWords("sgb-words.txt");

	std::cout << "Welcome to the 5-Letter Word Game!\n" << std::endl;

	bool playingGame = true;
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> listIndexRange(0, fiveLetterWords.size() - 1);

	while(playingGame)
	{
		const std::string word = fiveLetterWords[listIndexRange(rng)];
		bool guessing = true;

		while(guessing)
		{
			std::cout << "Enter your guess for the word or 'q' to give up: ";

			std::string guess;
			std::cin >> guess;
			
			for(char& c : guess)
			{
				c = tolower(c);
			}

			if(guess == "q")
			{
				break;
			}

			if(guess.size() != 5)
			{
				std::cout << "That's not 5 letters, this is the 5 letter word game learn to count! Try Again!" << std::endl;
				continue;
			}

			if(!IsWordInList(fiveLetterWords, guess))
			{
				std::cout << "That's not a word ya dunce! Try Again!" << std::endl;
				continue;
			}

			int score = MatchScore(word, guess);

			if(score == 10)
			{
				std::cout << "Good job, you got ir right!" << std::endl;
				guessing = false;
			}
			else if(score == 0)
			{
				std::cout << "Not even close, 0 points! Guess again!" << std::endl;
			}
			else
			{
				std::cout << "Wrong but I'll give ya " << score << " points for that one! Give it another go!" << std::endl;
			}
		}
		
		break;
	}


    return 0;
}

