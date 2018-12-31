/*
This is the console executable that makes use of the BullCow class
This acts as the view in an MVC pattern, and is responsible for all
user interaction. For game logic see the FBullCowGame class.
*/
#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

// to make syntax Unreal friendly
using FText = std::string;
using int32 = int;

void PrintIntro();
void PlayGame();
void PrintGameSummary();
FText GetValidGuess();
void PrintGuess(FText &Guess);
bool YesOrNoQuestion(FText);

// Globally Scoped Game Instance
FBullCowGame BCGame;

int main()
{
	do
	{
		PrintIntro();
		PlayGame();
	}
	while (YesOrNoQuestion("Do you want to play again? (Y/N) "));

	return 0;
}

// go through a single game to completion
void PlayGame()
{
	int32 Max_Tries = BCGame.GetMaxTries();
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= Max_Tries)
	{
		FText Guess = GetValidGuess();
		PrintGuess(Guess);

		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);
		std::cout << "Bulls: " << BullCowCount.Bulls;
		std::cout << "  Cows: " << BullCowCount.Cows << std::endl << std::endl;
	}
	PrintGameSummary();
	return;
}

// introduce the game
void PrintIntro()
{
	std::cout << std::endl << std::endl;
	BCGame.Reset();
	int32 WordLength = BCGame.GetWordLength();
	std::cout << "Welcome to Bulls and Cows, a fun word game.\n";
	std::cout << "Can you guess the " << WordLength;
	std::cout << " letter isogram I'm thinking of?\n";
	std::cout << std::endl;
	return;
}

// get a valid guess from the player
FText GetValidGuess()
{
	FText Guess = "";
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do
	{
		int32 CurrentTry = BCGame.GetCurrentTry();
		std::cout << "Guess " << CurrentTry << " of " << BCGame.GetMaxTries() << " -- Enter your guess: ";
		std::getline(std::cin, Guess);
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Too_Long:
			std::cout << "Your guess was too long. Please enter a " << BCGame.GetWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Too_Short:
			std::cout << "Your guess was too short. Please enter a " << BCGame.GetWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			if (YesOrNoQuestion("Your guess is not an isogram. Are you sure you want to submit it? (Y/N) "))
			{
				return Guess;
			}
			break;
		default:
			// assume the guess is valid
			return Guess;
		}
	} while (Status != EGuessStatus::OK);
	return Guess;
}

// prints guess back to player
void PrintGuess(FText &Guess)
{
	std::cout << "Your guess was " << Guess << std::endl;
	return;
}

// handle answers to yes or no questions
bool YesOrNoQuestion(FText Question)
{
	std::cout << Question;
	FText Response = "";
	std::getline(std::cin, Response);
	if (Response[0] == 'y' || Response[0] == 'Y')
	{
		return true;
	}
	else
	{
		return false;
	}
}

// print win or loss
void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "Congratulations! You guessed it!\n\n";
	}
	else
	{
		std::cout << "Better luck next time!\n\n";
	}
	return;
}
