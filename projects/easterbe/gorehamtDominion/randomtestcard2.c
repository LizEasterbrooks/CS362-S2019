/*******************************************************************************************************************
** CS Assignment 4 - randomtestcard2 ("Random Tests")															  **
** Name: Elizabeth Easterbrooks																					  **
** Date Due: 19 May 2019																							  **
** Description: 	random testing for the dominion card "council_room"; calls made to cardEffect in dominion.c		  **
** (the prototype for which is in dominion_helper.h), which in turn calls councilRoomEffect.						  **																							  **
**																												  **
** All other notes, comments, and documentation can be found in the body of the program below.					  **
*******************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"			//need for struct, enum, prototypes
#include "dominion_helpers.h"	//need for prototype of cardEffect

//reference: prototype for cardEffect
/*int cardEffect(int card, int choice1, int choice2, int choice3, 
	       struct gameState *state, int handPos, int *bonus);*/

#define TEST_MAX 20
#define MAX_PLAYERS 4			//this is duplicated from dominion.h
#define MIN_PLAYERS 2			//this is the logical mininum (Dominion is *not* Solitaire)
#define MAX_DECK 500

//modified from printState in randomtestcard1.c
void printState(struct gameState* previousState, struct gameState* currentState, int failCase)
{
	int deckDiscardBef;
	int deckDiscardAft;

	if (failCase == 1)
	{
		//cardEffect failed
		printf("Game states (before/after): ");
		printf("numPlayers: %d/%d\n", previousState->numPlayers, currentState->numPlayers);
		printf("whoseTurn: %d/%d\n", previousState->whoseTurn, currentState->whoseTurn);
		printf("player's handCount: %d/--> expect before + 3 - 1: %d\n", previousState->handCount[previousState->whoseTurn], currentState->handCount[previousState->whoseTurn]);
		printf("player's deckCount: %d/--> expect before - 1 (unless empty, then expect previous discard - 1): %d\n", previousState->deckCount[previousState->whoseTurn], currentState->deckCount[previousState->whoseTurn]);
		printf("player's discardCount: %d/--> expect no change (unless deck was empty or less than 3, then expect 0 discards): %d\n", previousState->discardCount[previousState->whoseTurn], currentState->discardCount[previousState->whoseTurn]);
		printf("playedCardCount: %d/--> expect before + 1: %d\n", previousState->playedCardCount, currentState->playedCardCount);
		//THIS PROBABLY NEEDS MORE THINGS!!!! --> out of time
	}
	else if (failCase == 2)
	{
		//hand size failed
		printf("FAIL: player's handCount: ");
		printf("handCount: %d/--> expect + 4 - 1: %d\n", previousState->handCount[previousState->whoseTurn], currentState->handCount[previousState->whoseTurn]);
	}
	else if (failCase == 3)
	{
		//cards played count failed
		printf("FAIL: playedCardCount: ");
		printf("playedCardCount: %d/--> expect + 1: %d\n", previousState->playedCardCount, currentState->playedCardCount);
	}
	else if (failCase == 4)
	{
		//deck + discards count failed (discard count WILL NOT change after cardEffect unless deck runs out during draws)
		deckDiscardBef = (previousState->deckCount[previousState->whoseTurn]) + (previousState->discardCount[previousState->whoseTurn]);
		deckDiscardAft = (currentState->deckCount[currentState->whoseTurn]) + (currentState->discardCount[currentState->whoseTurn]);
		printf("FAIL: deck/discard card count:\n");
		printf("Explanation: deckCount + discardCount before call should be four more than deckCount + discardCount after\n");
		printf("Before: deckCount (%d) + discardCount(%d) = %d\n", previousState->deckCount[previousState->whoseTurn], previousState->discardCount[previousState->whoseTurn], deckDiscardBef);
		printf("After: --> expect four less than before: deckCount (%d) + discardCount(%d) = %d\n", currentState->deckCount[currentState->whoseTurn], currentState->discardCount[currentState->whoseTurn], deckDiscardAft);
	}
	else //fail case 5
	{
		printf("FAIL: numBuys: ");
		printf("numBuys: %d/--> expect + 1: %d\n", previousState->numBuys, currentState->numBuys);
	}
}


void randomCouncilRoomTest()
{
	struct gameState randGame; //randomized dominion game state
	struct gameState rGameCpy; //copy of the game state before card is played for later comparison
	srand(time(NULL));
	int handPos;				   //position of the card to be "played" during the test; this is quasi random during game, player uses this index to select card for play 
	int card = council_room;	   //IF ENUM DOESN'T WORK THIS SHOULD BE "8"
	int choice1 = 0; 
	int choice2 = 0; 
	int choice3 = 0; 
	int bonus = 0;
	int i;
	int k;
	int m;
	int n;
	int p;
	for (i = 0; i < TEST_MAX; i++)
	{
		//create a fully random game state; REFERRED TO EXAMPLE FROM LECTURE 11
		//generate a number in a given range: number = (rand() % (<maxvalue> - <minvalue> + 1)) + <minvalue> --> straight from CS 161/162 textbook
		for (k = 0; k < sizeof(struct gameState); k++)
		{
			((char *)&randGame)[i] = rand() % 255 + 1; //fill with a "character" (byte) with a value between 1 and 255 (I'm avoiding the null character just because)
		}
		//generate random values in reasonable ranges for select fields of the game state
		randGame.numPlayers = (rand() % (MAX_PLAYERS - MIN_PLAYERS + 1)) + MIN_PLAYERS;		//max 4, min 2
		randGame.whoseTurn = rand() % randGame.numPlayers;		//max numPlayers, min 0; the indices representing the players in int player[MAX_PLAYERS]
		randGame.numBuys = (rand() % 5) + 1;						//number of buys will always be at least 1 in action phase; player may have added more in other actions (no cards remove buys?)
		for (m = 0; m < randGame.numPlayers; m++) 				//RANDOMIZE THE HANDCOUNT OF ALL THE PLAYERS
		{
			randGame.handCount[m] = (rand() % 200) + 1;			//FOR NOW, limiting the number of cards to between 200 and 1; max is 500, but unlikely to get above 12-15 due to action restrictions

		}
		handPos = (rand() % randGame.handCount[randGame.whoseTurn]) + 1; //between 1 and the number of cards in hand; not actually council_room
		for (n = 0; n < randGame.numPlayers; n++)						//NEED TO RANDOMIZE THE DECKCOUNT AND DISCARD COUNT OF ALL PLAYERS
		{
			randGame.deckCount[n] = rand() % MAX_DECK;				   //between 0 - 499; we're avoiding a max out situation for now
			if (randGame.deckCount[n] == 0)							   //if the deck is empty, then discard CANNOT be empty (logic of the game; not possible)
			{
				randGame.discardCount[n] = (rand() % MAX_DECK - 10 + 1) + 10; //between 10 - 500; 10 is minimum starting deck
			}
			else
			{
				randGame.discardCount[n] = rand() % MAX_DECK;				//between 0 - 499
			}
		}
		randGame.playedCardCount = rand() % MAX_DECK;						//between 0 - 499; not testing edge case of max played cards in this test

		//NEXT: copy the raw data of the entire struct into the holding struct
		memcpy(&rGameCpy, &randGame, sizeof(struct gameState));
		
		//THEN: call cardEffect for council_room, passing one of the game states, then compare previous state to state after call
		if (cardEffect(card, choice1, choice2, choice3, &randGame, handPos, &bonus) >= 0)
		{
			//The number of cards in player's hand should be + 4 - 1 (because played card is discarded)
			if (((rGameCpy.handCount[rGameCpy.whoseTurn] + 3) != randGame.handCount[rGameCpy.whoseTurn])) {
				printf("FAILURE IN TEST #%d: ", (i + 1));
				printState(&rGameCpy, &randGame, 2);
			}
			//The number of cards in the played pile (NOT DISCARDS) should be +1
			if (((rGameCpy.playedCardCount + 1) != randGame.playedCardCount)) {
				printf("FAILURE IN TEST #%d: ", (i + 1));
				printState(&rGameCpy, &randGame, 3);
			}
			//The number of cards in the player's deck plus discards should be three less than previous deck + discards
			//(discard count WILL NOT CHANGE after calling cardEffect unless deck runs out during card draws;
			//thus the sum of the two should reflect the number of the players cards not held in their hand)
			if ((((rGameCpy.deckCount[rGameCpy.whoseTurn]) + (rGameCpy.discardCount[rGameCpy.whoseTurn]) - 4) != ((randGame.deckCount[rGameCpy.whoseTurn]) + (randGame.discardCount[rGameCpy.whoseTurn])))) {
				printf("FAILURE IN TEST #%d: ", (i + 1));
				printState(&rGameCpy, &randGame, 4);
			}
			//the number of buys available to the player should be +1 than previous
			if ((rGameCpy.numBuys + 1) != randGame.numBuys)
			{
				printf("FAILURE IN TEST #%d: ", (i + 1));
				printState(&rGameCpy, &randGame, 5);
			}
			//for loop to check that each non-player has the right number of cards in their hand (+1)
			for (p = 0; p < randGame.numPlayers; p++)
			{
				if (p != rGameCpy.whoseTurn)											//if this isn't the player who played the card
				{
					if (((rGameCpy.handCount[p] + 1) != randGame.handCount[p])) {
						printf("FAILURE IN TEST #%d: ", (i + 1));
						printf("Player #%d has incorrect number of cards in hand:\n", (p + 1));
						//hand size failed for players who did not play card
						printf("FAIL: other's handCount: ");
						printf("handCount: %d/--> expect + 1 - 1: %d\n", randGame.handCount[p], rGameCpy.handCount[p]);
						//I *should* change printState to accept more arguments and call it here, that would be good form but OUT OF TIME!
					}
				}

			}
		}
		else 
		{
			printf("FAILURE IN TEST #%d: ", (i + 1));
			printf("FAIL: cardEffect returned with error (-1)");
			printState(&rGameCpy, &randGame, 1);
		}

		printf("\n");
	}
	printf("Random Testing of \"Council Room\" Card Complete\n(NOTE: only failed tests and states printed in results; if no stated failures, all tests passed)\n\n");
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
	printf("Random Test: \"Council Room\" card results:\n\n");
	randomCouncilRoomTest();
    return 0;
}