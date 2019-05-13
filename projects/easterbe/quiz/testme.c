#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

//To avoid the problem of not being able to free a dynamically allocated string inside of the calling function...
//Why is not being able to free a dynamically allocated string a problem? Well, first of all, if I dynamically
//allocate space for the string each time inputString is called inside testme, but testme never calls free on the
// string, then after running the loop many times (looping "infinitely") eventually we would run out of memory.
// So I either change testme to free the damn string, otherwise the string needs to be a static variable... 
// the only way that can occur is to have a global variable. Yuck!
#define MAX_STR 5

char randomStr[1000];

char inputChar()
{
    // DONE --> NEEDS TO COMPILE AND RUN BEFORE CONSIDER "complete"
	//generate a random character and return it
	//This is pretty straightforward and probably a piece of cake.
	//This *probably* won't change very much after we start messing around with it.
	//We have the freedom to pick which characters we get to iterate through, so... we'll see if that makes this easier
	//or harder to satisfy the conditions
	//Do I not need to call srand here since it is already being called in main?
	char randChar;
	//generate the randomChar here
	//generate a number in a given range: number = (rand() % (<maxvalue> - <minvalue> + 1) + <minvalue>) --> straight from CS 161/162 textbook
	randChar = (rand() % (125 - 32 + 1) + 32);
    return randChar;
}

char *inputString()
{
    // TODO: rewrite this function
	//I need to generate several random letters, insert them into a char* and send the pointer back
	//I guess that needs to be dynamically allocated then...? (Am I allowed to add a line to free in testme???)
	//For the first attempt I will limit the number of characters to five plus the null terminator.
	//I'll memset the whole thing with null terminators then for loop to generate lowercase chars for five indices
	//that should make it fairly likely for the error and exit state to occur in testme, and then after we acheive that
	//(or in five minutes) we can check the amount of coverage we got and go from there.
	//If we don't get close to 85% then we'll need to come back and make it less likely to get this "reset" string
	//so that the tests run longer...? Oof, we'll see.
	//This is *PROBABLY* the one we'll change the most (probably. no promises)
	char* inputStr = randomStr; //this is stupid... but what am I supposed to do if there is no way to free dynamically allocated memory? I'd run out before the infinite loop finished!
	//But this almost makes returning a pointer from this function retarded. I could just use the stupid global variable in the stupid testme...rawr...
	memset(inputStr, '\0', sizeof(char) * 1000);
	//generate random values for five characters of the string
	int i;
	for (i = 0; i < MAX_STR; i++)
	{
		inputStr[i] = (rand() % (116 - 101 + 1) + 101);
	} 
	return inputStr;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
