/*
The main game class. Implements all game logic as a controller,
no direct user interaction (no view logic).
*/
#pragma once
#include "FBullCowGame.h"
#include <unordered_set>
#include <math.h>
#include <map>

// to make syntax Unreal friendly
#define TMap std::map
using FString = std::string;
using int32 = int;

FBullCowGame::FBullCowGame()
{
	Reset();
	return;
}

int32 FBullCowGame::GetMaxTries() const
{
	return MyMaxTries;
}

int32 FBullCowGame::GetCurrentTry() const
{
	return MyCurrentTry;
}

int32 FBullCowGame::GetWordLength() const
{
	return HiddenWord.length();
}

bool FBullCowGame::IsGameWon() const
{
	return bGameIsWon;
}

bool FBullCowGame::IsIsogram(FString Word) const
{
	// create map
	TMap<char, bool> LetterSeen;
	// Loop through the word once
	for (auto Letter : Word)
	{
		// cast to lower case
		Letter = tolower(Letter);
		if (LetterSeen[Letter])
		{
			return false;
		}
		else
		{
			LetterSeen[Letter] = true;
		}
	}
	return true;
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (int32(Guess.length()) < GetWordLength())
	{
		return EGuessStatus::Too_Short;
	}
	else if (int32(Guess.length()) > GetWordLength())
	{
		return EGuessStatus::Too_Long;
	}
	else if (!IsIsogram(Guess))
	{
		return EGuessStatus::Not_Isogram;
	}
	else
	{
		return EGuessStatus::OK;
	}
}

// recieves a valid guess, and increments try
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	// increment try number
	MyCurrentTry++;

	// set up return variable
	FBullCowCount BullCowCount;

	// sets of already matched letters
	std::unordered_set<char> BullSet = {};
	std::unordered_set<char> CowSet = {};
	
	int32 WordLength = HiddenWord.length(); // assuming guess length and hidden word length are equal
	for (int32 i = 0; i < WordLength; i++)
	{
		Guess[i] = tolower(Guess[i]);
		if (BullSet.count(Guess[i]) > 0)
		{
			continue;
		}
		if (Guess[i] == HiddenWord[i])
		{
			BullCowCount.Bulls++;
			BullSet.insert(Guess[i]);
			if (CowSet.count(Guess[i]) > 0)
			{
				BullCowCount.Cows--;
			}
			continue;
		}
		for (int32 j = 0; j < WordLength; j++)
		{
			if (CowSet.count(Guess[i]) > 0)
			{
				break;
			}
			if (Guess[i] == HiddenWord[j])
			{
				BullCowCount.Cows++;
				CowSet.insert(Guess[i]);
				break;
			}
		}
	}
	if (BullCowCount.Bulls == WordLength)
	{
		bGameIsWon = true;
	}
	return BullCowCount;
}

void FBullCowGame::Reset()
{
	const FString HIDDEN_WORD = "planet"; // this MUST be an isogram
	const int32 MAX_TRIES = int32(HIDDEN_WORD.length() * 2) - int32(ceil(HIDDEN_WORD.length() / 2));
	
	MyMaxTries = MAX_TRIES;
	MyCurrentTry = 1;
	HiddenWord = HIDDEN_WORD;
	bGameIsWon = false;
	return;
}
