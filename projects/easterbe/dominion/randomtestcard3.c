/*******************************************************************************************************************
** CS Assignment 4 - randomtestcard3 ("Random Tests")															  **
** Name: Elizabeth Easterbrooks																					  **
** Date Due: 19 May 2019																							  **
** Description: 	random testing for the dominion card "adventurer"												  **
**																												  **
**					!!!! CODE INCOMPLETE - MAY NOT COMPILE (?) - WILL NOT RUN AS INTENDED!!!!!					  **
**																												  **
** All other notes, comments, and documentation can be found in the body of the program below.					  **
*******************************************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "dominion.h"			//need for struct, enum, prototypes
#include "dominion_helpers.h"	//need for prototype of cardEffect

/*int cardEffect(int card, int choice1, int choice2, int choice3, 
	       struct gameState *state, int handPos, int *bonus);*/

#define TEST_MAX 20
#define MAX_PLAYERS 4			//this is duplicated from dominion.h; original rule of game, modern games can have more players(?)
#define MIN_PLAYERS 2			//this is the logical mininum (Dominion is *not* Solitaire)
#define MAX_DECK 500

//NOTE: THE PROTOTYPE FOR cardEffect IS IN dominion_helpers

//MODIFY THIS FROM THE SMITHY VERSION
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
		printf("handCount: %d/--> expect before + 3 - 1: %d\n", previousState->handCount[previousState->whoseTurn], currentState->handCount[previousState->whoseTurn]);
		printf("deckCount: %d/--> expect before - 1 (unless empty, then expect previous discard - 1): %d\n", previousState->deckCount[previousState->whoseTurn], currentState->deckCount[previousState->whoseTurn]);
		printf("discardCount: %d/--> expect no change (unless deck was empty or less than 3, then expect 0 discards): %d\n", previousState->discardCount[previousState->whoseTurn], currentState->discardCount[previousState->whoseTurn]);
		printf("playedCardCount: %d/--> expect before + 1: %d\n", previousState->playedCardCount, currentState->playedCardCount);
	}
	else if (failCase == 2)
	{
		//hand size failed
		printf("FAIL: handCount: ");
		printf("handCount: %d/--> expect + 3 - 1: %d\n", previousState->handCount[previousState->whoseTurn], currentState->handCount[previousState->whoseTurn]);
	}
	else if (failCase == 3)
	{
		//cards played count failed
		printf("FAIL: playedCardCount: ");
		printf("playedCardCount: %d/--> expect + 1: %d\n", previousState->playedCardCount, currentState->playedCardCount);
	}
	else
	{
		//deck + discards count failed (discard count WILL NOT change after cardEffect unless deck runs out during draws)
		deckDiscardBef = (previousState->deckCount[previousState->whoseTurn]) + (previousState->discardCount[previousState->whoseTurn]);
		deckDiscardAft = (currentState->deckCount[currentState->whoseTurn]) + (currentState->discardCount[currentState->whoseTurn]);
		printf("FAIL: non-hand card count:\n");
		printf("Explanation: deckCount + discardCount before call should be three more than deckCount + discardCount after\n");
		printf("Before: deckCount (%d) + discardCount(%d) = %d\n", previousState->deckCount[previousState->whoseTurn], previousState->discardCount[previousState->whoseTurn], deckDiscardBef);
		printf("After: --> expect three less than before: deckCount (%d) + discardCount(%d) = %d\n", currentState->deckCount[currentState->whoseTurn], currentState->discardCount[currentState->whoseTurn], deckDiscardAft);
	}
}


void randomAdventurerTest()
{
	struct gameState randGame; //randomized dominion game state
	struct gameState rGameCpy; //copy of the game state before card is played for later comparison
	srand(time(NULL));
	int handPos;				   //position of the card to be "played" during the test; this is quasi random during game, player uses this index to select card for play 
	int card = adventurer;		   //IF ENUM DOESN'T WORK THIS SHOULD BE "7"
	int choice1 = 0; 
	int choice2 = 0; 
	int choice3 = 0; 
	int bonus = 0;
	int i;
	int k;
	for (i = 0; i < TEST_MAX; i++)
	{
		//THIS IS NOT ANY DIFFERENT FROM SMITHY VERSION --> GO THROUGH EACH FIELD OF THE STRUCT AND CONSIDER IF NEEDS CUSTOM RANDOMIZATION
		//create a fully random game state; REFERRED TO EXAMPLE FROM LECTURE 11
		//generate a number in a given range: number = (rand() % (<maxvalue> - <minvalue> + 1)) + <minvalue> --> straight from CS 161/162 textbook
		for (k = 0; k < sizeof(struct gameState); k++)
		{
			((char *)&randGame)[i] = rand() % 255 + 1; //fill with a "character" (byte) with a value between 1 and 255 (I'm avoiding the null character just because)
		}
		//generate random values in reasonable ranges for select fields of the game state
		randGame.numPlayers = (rand() % (MAX_PLAYERS - MIN_PLAYERS + 1)) + MIN_PLAYERS;		//max 4, min 2
		//constrain the values in the int array supplyCount? AS FAR AS I KNOW AT THIS TIME: NO
		//constrain the values in the int array embargoTokens? AS FAR AS I KNOW AT THIS TIME: NO
		//constrain the values in the int outpostPlayed? AS FAR AS I KNOW AT THIS TIME: NO
		randGame.whoseTurn = rand() % randGame.numPlayers; //max numPlayers, min 0; the indices representing the players in int player[MAX_PLAYERS]
		//constrain the values in the int phase? Technically, yes since the card only be played during the action phase, but for the purposed of the test, NO, not necessary
		//randGame.numActions = 1; //start with the minimum for each turn; if a card adds actions, so be it; NOT NEEDED FOR SMITHY
		//constrain the values in the int coins? AS FAR AS I KNOW AT THIS TIME: NO
		//constrain the values in the int numBuys? Technically, yes, but for this case NO; when a card adds buys for a player, test it.
		//OKAY, SO... the next couple of fields in the struct are a little more complicated and will involve for this test special randomization...?
		//We need whoever's turn it is to have a hand of a random size and that hand to generally have cards of a reasonable range (?) AND...
		//a random card from that selection needs to be "smithy"... so after randomizing the size, contents, and the card position, then 
		//we have to NOT RANDOMLY assign a smithy card to that position (if we're calling card effect... but if not... not necessary?)
		//INSERT THAT CODE HERE --> covers hand, handCount, and handPos
		randGame.handCount[randGame.whoseTurn] = (rand() % 200) + 1; //FOR NOW, limiting the number of cards to between 200 and 1; max is 500, but unlikely to get above 12-15 due to action restrictions
		handPos = (rand() % randGame.handCount[randGame.whoseTurn]) + 1; //between 1 and the number of cards in hand; NO SMITHY CARD WILL EXIST HERE WHILE smithyEffect IS CALLED DIRECTLY (in case of cardEffect, MUST make this card the enum "smithy")
		//And... as far as the deck and discard piles go... for drawCard and discardCard to not freak out inside smithyEffect
		//we need to have a deck of a certain size; if it is 0 then the discards cannot be 0 and must be of a certain size
		//I don't think the actual cards themselves will matter at this point
		//INSERT THAT CODE HERE --> covers deck, deckCount, discard, discardCount
		randGame.deckCount[randGame.whoseTurn] = rand() % MAX_DECK; //between 0 - 499; we're avoiding a max out situation for now
		if (randGame.deckCount[randGame.whoseTurn] == 0) //if the deck is empty, then discard CANNOT be empty (logic of the game; not possible)
		{
			randGame.discardCount[randGame.whoseTurn] = (rand() % MAX_DECK - 10 + 1) + 10; //between 10 - 500; 10 is minimum starting deck
		}
		else
		{
			randGame.discardCount[randGame.whoseTurn] = rand() % MAX_DECK; //between 0 - 499
		}
		//And lastly for playedCards and playedCardCount...
		//just like with deck and hand, it's not so important to care about the contents, but count needs to be in a reasonable range (much less than max)
		//INSERT THAT CODE HERE --> covers playedCards, playedCardCount
		randGame.playedCardCount = rand() % MAX_DECK; //between 0 - 499; not testing edge case of max played cards in this test

		//NEXT: copy the raw data of the entire struct into the holding struct
		memcpy(&rGameCpy, &randGame, sizeof(struct gameState));
		
		//THEN: call cardEffect for adventurer
		//!!!!!!THIS IS NOT ANY DIFFERENT FROM SMITHY VERSION --> MODIFY AND ADD ADDITIONAL ORACLE CODE!!!
		if (cardEffect(card, choice1, choice2, choice3, &randGame, handPos, &bonus) >= 0)
		{
			//The number of cards in hand should be + 3 - 1 (because played card is discarded)
			if (((rGameCpy.handCount[rGameCpy.whoseTurn] + 2) != randGame.handCount[rGameCpy.whoseTurn])) {
				printf("FAILURE IN TEST #%d:\n", (i + 1));
				printState(&rGameCpy, &randGame, 2);
			}
			//The number of cards in the played pile (NOT DISCARDS) should be +1
			if (((rGameCpy.playedCardCount + 1) != randGame.playedCardCount)) {
				printf("FAILURE IN TEST #%d:\n", (i + 1));
				printState(&rGameCpy, &randGame, 3);
			}
			//The number of cards in deck plus discards should be three less than previous deck + discards
			//(discard count WILL NOT CHANGE after calling cardEffect unless deck runs out during card draws;
			//thus the sum of the two should reflect the number of the players cards not held in their hand)
			if ((((rGameCpy.deckCount[rGameCpy.whoseTurn]) + (rGameCpy.discardCount[rGameCpy.whoseTurn]) - 3) != ((randGame.deckCount[rGameCpy.whoseTurn]) + (randGame.discardCount[rGameCpy.whoseTurn])))) {
				printf("FAILURE IN TEST #%d:\n", (i + 1));
				printState(&rGameCpy, &randGame, 4);
			}
		}
		else 
		{
			printf("FAIL: cardEffect returned with error (-1)");
			printState(&rGameCpy, &randGame, 1);
		}

		printf("\n");
		//LASTLY: write several lines of oracle code to compare the game state before and after
		//The number of cards in hand should be +3 - 1 (because played card is discarded)
		//The number of cards in the played pile should be +1
		//The number of cards in the deck should be -3 (if deck was >= 3; otherwise three less than prev discards)
		//All other places in state should stay the same
		//Any failure of these conditions should be printed as FAIL: <the type of fail here; the state of the game at failure>

	}
	printf("Random Testing of \"Adventurer\" Card Complete\n(NOTE: only failed tests and states printed in results; if no stated failures, all tests passed)\n\n");
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
	printf("Random Test: \"Adventurer\" card results:\n\n");
	printf("(WARNING: randomAdventurerTest function not complete; randomtestcard3.c compiles but WILL NOT run tests!!\n\n"); //REMOVE AFTER COMPLETION
	//randomAdventurerTest();								 //NOT COMPLETE; DO NOT UN-COMMENT!!!!!!!
    return 0;
}