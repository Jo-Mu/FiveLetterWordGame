﻿// FiveLetterWordGame.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <random>
#include <cctype>
#include <algorithm>

//returns a string of executable path
std::string GetExePath()
{
	char result[MAX_PATH];
	return std::string(result, GetModuleFileNameA(NULL, result, MAX_PATH));
}

//Returns a vector of strings containing words from a file with the given file name
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

//Returns true if the given word guess is in the given word list (a vector of strings)
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

//Returns an int vector with a size of 26 (for each letter in the aplhabet) containing the amount of times a letter appear in the given word.
//Each index represents a letter of the aplhabet from 0 being 'a' to 26 being 'z' 
std::vector<int> GetCharBucket(const std::string& word)
{
	std::vector<int> charBucket(26, 0);

	for(const char& c : word)
	{
		charBucket[c - 'a']++;
	}

	return charBucket;
}

//Returns an int score that is how close the given guess is to the given word
//1 point for each letter both words have and another for each letter that is in the same position in both
int MatchScore(const std::string& word, const std::string& guess)
{
	int score = 0;

	//Char buckets formed for the mystery word and the guess.
	const auto wordBucket = GetCharBucket(word);
	const auto guessBucket = GetCharBucket(guess);

	//Adds the amount of letters that both words have (multiples included) to score.
	for(int i = 0; i < 26; i++)
	{
		score += (std::min)(wordBucket[i], guessBucket[i]);
	}

	//Adds an extra point for each letter in the guess that is in the same position as the mystery word.
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
	//Load words from given file.
	std::vector<std::string> fiveLetterWords = LoadWords("sgb-words.txt");

	std::cout << "Welcome to the 5-Letter Word Game!\n" << std::endl;

	bool playingGame = true;
	std::mt19937 rng(std::random_device{}());
	std::uniform_int_distribution<int> listIndexRange(0, fiveLetterWords.size() - 1);

	//Game will continue until the player quits.
	while(playingGame)
	{
		//Word selected at random from the list of words.
		const std::string word = fiveLetterWords[listIndexRange(rng)];
		bool guessing = true;

		//Continue guessing the word until the player gets it correct or gives up.
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
				std::cout << "\nThat's not 5 letters, this is the 5 letter word game learn to count! Try Again!\n" << std::endl;
				continue;
			}

			if(!IsWordInList(fiveLetterWords, guess))
			{
				std::cout << "\nThat's not a word ya dunce! Try Again!\n" << std::endl;
				continue;
			}

			int score = MatchScore(word, guess);

			if(score == 10)
			{
				std::cout << "\nGood job, you got ir right!\n" << std::endl;

				guessing = false;
			}
			else if(score == 0)
			{
				std::cout << "\nNot even close, 0 points! Guess again!\n" << std::endl;
			}
			else
			{
				std::cout << "\nWrong but I'll give ya " << score << " points for that one! Give it another go!\n" << std::endl;
			}
		}

		//Player can decide whether to replay the game with a new word or to quit, exiting the program.
		while(playingGame)
		{
			std::cout << "Enter 'retry' to guess another word or 'quit' to exit the game: ";

			std::string input;
			std::cin >> input;

			for(char& c : input)
			{
				c = tolower(c);
			}

			if(input == "quit")
			{
				playingGame = false;
			}
			else if(input == "retry")
			{
				system("CLS");
				break;
			}
			else
			{
				std::cout << "\nPlease enter 'retry' or quit'.\n" << std::endl;
			}
		}
	}

    return 0;
}

