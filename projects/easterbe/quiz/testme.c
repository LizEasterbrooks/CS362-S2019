#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define MAX_STR 5

//To avoid the problem of not being able to free a dynamically allocated string inside of testme()...
char randomStr[1000];
// Why is not being able to free a dynamically allocated string a problem? Well, first of all, if I dynamically
// allocate space for the string each time inputString is called inside testme, but testme never calls free on the
// string, then after running the loop many times (looping "infinitely") eventually we would run out of memory.
// So I either change testme to free the damn string, otherwise the string needs to be a static variable... 
// the only way that can occur is to have a global variable. Yuck!


char inputChar()
{

	char randChar;
	//generate a number in a given range: number = (rand() % (<maxvalue> - <minvalue> + 1) + <minvalue>) --> straight from CS 161/162 textbook
	randChar = (rand() % (125 - 32 + 1) + 32);
    return randChar;
}

char *inputString()
{
	char* inputStr = randomStr;
	memset(inputStr, '\0', sizeof(char) * 1000);
	int i;
	//generate random values for five characters of the string
	for (i = 0; i < MAX_STR; i++) 
	{
		inputStr[i] = (rand() % (116 - 101 + 1) + 101);
	} 
	//if desired: make the length of the string variable by generating a random number (between 1-999) to bind the loop
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
