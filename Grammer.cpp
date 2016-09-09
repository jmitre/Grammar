#include <iostream>
#include <string>
#include <stdlib.h>

#include "Symbols.h"
#include "Parser.h"



using namespace std;

NonTerminal **Grammer;
Symbol **FirstSet;
Symbol **FollowSet;
bool *SL1;

void getNonterminals();
void printDataStructure();
void getGrammerRules();
void CheckForEps();
int countEps(Symbol *grammerRule);//return -1 if there are less than n-1 eps, return 0 if there are n eps and return 1 if there are n-1 eps
bool GrammerRule1String(Symbol *GrammerRule);
bool NonTerm1String(NonTerminal *NTerm);
void hasLengthOne();
void calculateFirstSets();
void calculateFirstSets_NonTerminal(NonTerminal *NonTerm, int ArrayPlace);
void calculateFirstSets_Grammer(Symbol *currentSymbol, int ArrayPlace);
void addToFirstSet(Symbol *SymbolToAdd, int ArrayPlace);
void printFirstSets();
bool isDuplicate(string s, int arrayplace);

void wasVisitedToFalse();

void printFollowSets();
int indexOf(Symbol *S);
void addToFollowSet(Symbol *SymbolToAdd, int ArrayPlace);
void FirstToFollowSet();
void calculateFollowSets();
bool isDuplicateForFollow(string s, int arrayplace);
void addFirstToFollow(Symbol *First, Symbol *Second);
void addFollowToFollow(Symbol *First, Symbol *Second);
void FollowToFollow();

void addFirstToFirst(Symbol *First, Symbol *Second);
void addEpsFirstToFirst(Symbol *First, Symbol *Second);

Symbol *from = NULL;

int timesInA = 0;

int main(int arg, char** argA)//
{
	getNonterminals();

	getGrammerRules();

	printDataStructure();

	CheckForEps();

	int task;
	task = atoi(argA[1]);

	switch (task)
	{
	case 1:
		hasLengthOne();
		break;
	case 2:
		calculateFirstSets();
		printFirstSets();
		break;
	case 3:
		calculateFirstSets();
		calculateFollowSets();
		printFollowSets();
		break;
	default:
		cout << "ERROR: this task is not recognized";
	}




	/*

	printDataStructure();
	hasLengthOne();
	calculateFirstSets();
	printFirstSets();
	calculateFollowSets();
	printFollowSets();

	*/











	return 0;

}

void getNonterminals()
{
	string content;
	getline(cin, content);

	NonTerminal *head = parseNonTerminals(content);

	Grammer = new NonTerminal*[listLength];
	FirstSet = new Symbol*[listLength];
	FollowSet = new Symbol*[listLength];
	SL1 = new bool[listLength];

	for (int i = listLength - 1; i >= 0; i--)
	{

		Grammer[i] = head;
		FirstSet[i] = NULL;
		FollowSet[i] = NULL;
		SL1[i] = false;
		head = head->getNext();
	}

}

void getGrammerRules()
{


	parseGrammer(Grammer);

	return;
}


void printDataStructure()
{
	cout << "\nMY DATA STRUCTURE:\n";
	Symbol *GrammerRule;
	Symbol *temp = NULL;
	for (int i = 0; i < listLength; i++)
	{
		if (Grammer[i]->hasEpsilon())
			cout << "True ";
		else
			cout << "false ";
		cout << Grammer[i]->getContent() << "->";
		GrammerRule = Grammer[i]->getNextGrammerRule();
		temp = GrammerRule;
		while (GrammerRule != NULL)
		{
			cout << " " << countEps(GrammerRule) << " ";
			if (countEps(GrammerRule) == 0)
			{
				cout << "HEEERRRRREEEE/nEERRRREEEE";
			}

			cout << temp->getContent() << " ";


			while (temp->getNextSymbol() != NULL)
			{

				temp = temp->getNextSymbol();
				cout << temp->getContent() << " ";
			}


			GrammerRule = GrammerRule->getNextGrammerRule();
			temp = GrammerRule;

			if (GrammerRule != NULL)
				cout << "|";
		}

		cout << "\n";
	}
}

void CheckForEps()
{
	Symbol *GrammerRule, *temp;
	for (int i = 0; i < listLength; i++)
	{
		GrammerRule = Grammer[i];

		do{
			GrammerRule = GrammerRule->getNextGrammerRule();

			if (GrammerRule->Epsilon())
			{
				Grammer[i]->setHasEpsilon(true);
				break;
			}

		} while (GrammerRule->getNextGrammerRule() != NULL);
	}

	int count=0, counteps=0;

	for (int j = 0; j < listLength; j++)
	for (int i = 0; i < listLength; i++)
	{
		GrammerRule = Grammer[i]->getNextGrammerRule();

		while (GrammerRule)
		{
			temp = GrammerRule;
			while (temp)
			{
				count++;
				if (temp->Terminal() == false && temp->Epsilon() == false)
				if (((NonTerminal*)temp)->getMySymbol()->hasEpsilon())
					counteps++;

				temp = temp->getNextSymbol();
			}
			if (count != 0 && count == counteps)
				Grammer[i]->setHasEpsilon(true);
			count = 0;
			counteps = 0;
			GrammerRule = GrammerRule->getNextGrammerRule();
		}
	}
}





int countEps(Symbol *grammerRule)//return -1 if there are less than n-1 eps, return 0 if there are n eps and return 1 if there are n-1 eps
{
	int n = 0;
	int eps= 0;
	NonTerminal *Nont;

	do
	{
		n++;

		if (grammerRule->Terminal() == false && grammerRule->Epsilon() == false)
		{
			Nont = (NonTerminal*)grammerRule;
			Nont = Nont->getMySymbol();
			if (Nont->hasEpsilon() == true)//wrong!!!!!!!!!!!!! MY SYMBOL IS EPSILON????
				eps++;
		}

		grammerRule = grammerRule->getNextSymbol();

	} while (grammerRule != NULL);
	if (eps != 0)
	{
		if (n == eps)
			return 0;
		if (n - 1 == eps)
		{
			return 1;
		}
	}
	return -1;
}

void hasLengthOne()
{
	NonTerminal *Nont;

	Symbol *GrammerRule, *temp;
	for (int j = 0; j < listLength; j++)
	for (int i = 0; i < listLength ; i++)
	{
		Nont = Grammer[i];
		GrammerRule = Nont->getNextGrammerRule();
		temp = GrammerRule;
		while (GrammerRule)
		{
			if (GrammerRule->getNextSymbol() == NULL)
			{
				if (temp->Terminal() == true)//if only one terminal in grammer
					SL1[i] = true;
				if (temp->Terminal() == false && temp->Epsilon() == false) // if only 1 non terminal in grammer
				if (SL1[indexOf(temp)] == true)
				{
					SL1[i] = true;
				}
			}
			else
			if (countEps(GrammerRule)==0)
				while (temp)
				{
					if (temp->Terminal() == false && temp->Epsilon()==false)
						if (SL1[indexOf(temp)] == true)
						{
							SL1[i] = true;
						}
					temp = temp->getNextSymbol();

				}
				else
				if (countEps(GrammerRule) == 1)
					while (temp)
					{
						if (temp->Terminal() == true)
						{
							SL1[i] = true;
						}

						if (temp->Terminal() == false && temp->Epsilon() == false)
						{
							if (SL1[indexOf(temp)] == true)
							{
								SL1[i] = true;
							}
						}



						temp = temp->getNextSymbol();

					}


			GrammerRule = GrammerRule->getNextGrammerRule();
			temp = GrammerRule;
		}


	}

	for (int i = 0; i < listLength; i++)
	{
		cout << Grammer[i]->getContent() << ": ";
		if (SL1[i] == true)
			cout << "YES\n";
		else
			cout << "NO\n";
	}
}

void calculateFirstSets()
{
	NonTerminal *Nont;
	bool hasEps;
	Epsilon *eps;

	Symbol *GrammerRule, *temp;
	for (int j = 0; j < listLength; j++)
	for (int i = 0; i < listLength; i++)
	{
		Nont = Grammer[i];
		GrammerRule = Nont->getNextGrammerRule();
		temp = GrammerRule;
		while (GrammerRule != NULL)
		{
			while (temp != NULL)
			{
				if (temp->Terminal() == true || temp->Epsilon() == true)
					addToFirstSet(temp, i);

				if (temp->Terminal() == false && temp->Epsilon() == false)
					addFirstToFirst(temp, Nont);

				if (temp->Terminal() == true  || temp ->Epsilon() == true)//cant have eps
				{
					hasEps = false;
					break;
				}

				if (((NonTerminal*)temp)->getMySymbol()->hasEpsilon() == false)//if it doesnt have eps
				{
					hasEps = false;
					break;
				}


				hasEps = true;

				temp = temp->getNextSymbol();
			}

			if (hasEps == true)
			{
				//add eps to Nont
				eps = new Epsilon();
				addToFirstSet(eps, i);
			}

			GrammerRule = GrammerRule->getNextGrammerRule();
			temp = GrammerRule;
		}


	}

}

void calculateFirstSets_NonTerminal(NonTerminal *NonTerm , int ArrayPlace)//
{

	NonTerm->setWasVisited(true);
	Symbol *CurrentRule = NonTerm->getNextGrammerRule();

	do
	{
		calculateFirstSets_Grammer(CurrentRule, ArrayPlace);

		CurrentRule = CurrentRule->getNextGrammerRule();

	}while (CurrentRule != NULL);

}

void calculateFirstSets_Grammer(Symbol *currentSymbol, int ArrayPlace )
{

	int i = 0;
	do
	{
		if (i != 0)// you should not itterate on turn 1
			currentSymbol = currentSymbol->getNextSymbol();

		if (currentSymbol->Terminal())
		{
			addToFirstSet(currentSymbol, ArrayPlace);
		}
		else//it is a nonTerm
		{
			if (currentSymbol->Epsilon() == true)
			{
				addToFirstSet(currentSymbol, ArrayPlace);
			}
			else
			if (((NonTerminal*)currentSymbol)->getMySymbol()->getwasVisited() == false)
				calculateFirstSets_NonTerminal(((NonTerminal*)currentSymbol)->getMySymbol() , ArrayPlace);//can result in an infinite loop//WRONG!!!!!!!!
			//else cout << "IT WAS VISITED \n";
		}

		i++;

	} while (currentSymbol->Terminal() == false && currentSymbol->Epsilon() == false && ((NonTerminal*)currentSymbol)->getMySymbol()->hasEpsilon() == true && currentSymbol->getNextSymbol() != NULL);//could cause and error depending on how && works

}

void addFirstToFirst(Symbol *First, Symbol *Second)
{
	int FirstSPlace = indexOf(First);//First Set place
	int SecondPlace = indexOf(Second);//Follow set place

	Symbol *temp = FirstSet[FirstSPlace];

	while (temp != NULL)
	{
			if (isDuplicate(temp->getContent(), SecondPlace) == false && temp->Epsilon() == false)
				addToFirstSet(temp, SecondPlace);

		temp = temp->getNextSymbol();
	}
}



void addToFirstSet(Symbol *SymbolToAdd, int ArrayPlace)
{
	Symbol *add = NULL, *temp = FirstSet[ArrayPlace];
	if (SymbolToAdd->Terminal() == true)
		add = new Terminal(SymbolToAdd->getContent());
	else
	if (SymbolToAdd->Terminal() == false && SymbolToAdd->Epsilon() == true)
	{
		add = new Epsilon();
	}

	if (FirstSet[ArrayPlace] == NULL)//havent initialized array to NULL
	{
		FirstSet[ArrayPlace] = add;

		return;
	}
	else
	{
		//If add belongs at the begining
		if (add->getContent().compare(FirstSet[ArrayPlace]->getContent()) < 0)
		{
			add->setNextSymbol(FirstSet[ArrayPlace]);
			FirstSet[ArrayPlace] = add;

			return;
		}
		else
		while (temp->getNextSymbol() != NULL)
		{
			if (add->getContent().compare(temp->getNextSymbol()->getContent()) < 0)//add to next spot
			{
				if (isDuplicate(add->getContent(), ArrayPlace) == false)
				{
					add->setNextSymbol(temp->getNextSymbol());
					temp->setNextSymbol(add);

				}

				return;
			}

			temp = temp->getNextSymbol();

		}
		//add at end
		if (temp->getNextSymbol() == NULL && isDuplicate(add->getContent(), ArrayPlace) == false)
			temp->setNextSymbol(add);

	}

}

void printFirstSets()
{
	Symbol * temp;
	for (int i = 0; i < listLength; i++)//change back to listLength
	{
		temp = FirstSet[i];
		cout << "FIRST(" << Grammer[i]->getContent() << ") = {";
		while (temp != NULL)
		{
			cout << " " << temp->getContent();
			if (temp->getNextSymbol()!=NULL)
				cout << ",";
			temp = temp->getNextSymbol();
		}
		cout << " }";
		cout << "\n";
	}


}

bool isDuplicate(string s , int arrayplace)
{
	Symbol *temp;
	temp = FirstSet[arrayplace];

	while (temp != NULL)
	{
		if (temp->getContent().compare(s) == 0)
			return true;

		temp = temp->getNextSymbol();
	}



	return false;
}

void calculateFollowSets()
{
	//adds end of file to the top Non Terminal
	EndOfFile *add = new EndOfFile();
	FollowSet[0] = add;

	for (int i = 0; i < listLength*listLength; i++)
	{
		FirstToFollowSet();
		FollowToFollow();

	}




}

void FirstToFollowSet()
{
	Symbol *First;
	Symbol *Second;

	Symbol *grammerRule;

	for (int i = 0; i < listLength; i++)
	{
		grammerRule = Grammer[i]->getNextGrammerRule();

		while (grammerRule != NULL)
		{
			First = grammerRule;
			while (First->getNextSymbol() != NULL)
			{
				Second = First->getNextSymbol();
				if (First->Terminal() == false && First->Epsilon() == false)//First is a non Terminal
				{
					if (Second->Terminal() == false && Second->Epsilon() == false)//Non Terminal: add first set of Second to first set of first
					{
						addFirstToFollow(First, Second);
					}
					if (Second->Terminal() == true)//add the non Terminal to first set of First
					{
						addToFollowSet(Second, indexOf(First));
					}
				}
				First = First->getNextSymbol();
			}
			grammerRule = grammerRule->getNextGrammerRule();
		}
	}
}




void addToFollowSet(Symbol *SymbolToAdd, int ArrayPlace)
{
	Symbol *add = NULL, *temp = FollowSet[ArrayPlace];
	if (SymbolToAdd->Terminal() == true)
		add = new Terminal(SymbolToAdd->getContent());
	else
	if (SymbolToAdd->Terminal() == false && SymbolToAdd->Epsilon() == true)
	{
		add = new Epsilon();
	}
	else
	if (SymbolToAdd->getContent().compare("$") == 0)
		add = new EndOfFile();

	if (FollowSet[ArrayPlace] == NULL)//havent initialized array to NULL
	{
		FollowSet[ArrayPlace] = add;

		return;
	}
	else
	{
		//If add belongs at the begining
		if (add->getContent().compare(FollowSet[ArrayPlace]->getContent()) < 0)
		{
			add->setNextSymbol(FollowSet[ArrayPlace]);
			FollowSet[ArrayPlace] = add;

			return;
		}
		else
		while (temp->getNextSymbol() != NULL)
		{
			if (add->getContent().compare(temp->getNextSymbol()->getContent()) < 0)//add to next spot
			{
				if (isDuplicateForFollow(add->getContent(), ArrayPlace) == false)
				{
					add->setNextSymbol(temp->getNextSymbol());
					temp->setNextSymbol(add);

				}

				return;
			}

			temp = temp->getNextSymbol();

		}
		//add at end
		if (temp->getNextSymbol() == NULL && isDuplicateForFollow(add->getContent(), ArrayPlace) == false)
			temp->setNextSymbol(add);

	}

}

int indexOf(Symbol *S)
{

	for (int i = 0; i < listLength; i++)
	{
		if (Grammer[i]->getContent().compare(S->getContent()) == 0)
			return i;
	}

	return -1;
}


void printFollowSets()
{
	Symbol * temp;
	for (int i = 0; i < listLength; i++)//change back to listLength
	{
		temp = FollowSet[i];
		cout << "FOLLOW(" << Grammer[i]->getContent() << ") = {";
		while (temp != NULL)
		{
			cout << " " << temp->getContent();
			if (temp->getNextSymbol() != NULL)
				cout << ",";
			temp = temp->getNextSymbol();
		}
		cout << " }";
		cout << "\n";
	}


}

void addFirstToFollow(Symbol *First, Symbol *Second)
{
	int FirstSPlace = indexOf(Second);//First Set place
	int FollowSPlace = indexOf(First);//Follow set place

	Symbol *temp = FirstSet[FirstSPlace];

	while (temp != NULL)
	{
		if (temp->Epsilon() != true)
		{
			if (isDuplicateForFollow(temp->getContent(), FollowSPlace) == false)
				addToFollowSet(temp, FollowSPlace);
		}

		temp = temp->getNextSymbol();
	}
}

bool isDuplicateForFollow(string s, int arrayplace)
{
	Symbol *temp;
	temp = FollowSet[arrayplace];

	while (temp != NULL)
	{
		if (temp->getContent().compare(s) == 0)
			return true;

		temp = temp->getNextSymbol();
	}



	return false;
}

void FollowToFollow()
{
	NonTerminal *CurrentNonTerminal;
	Symbol *CurrentGrammerRule, *CurrentSymbol;

	for (int i = 0; i < listLength; i++)
	{
		CurrentNonTerminal = Grammer[i];
		CurrentGrammerRule = CurrentNonTerminal->getNextGrammerRule();

		while (CurrentGrammerRule != NULL)
		{
			CurrentSymbol = CurrentGrammerRule;
			while (CurrentSymbol != NULL)
			{
				if (CurrentSymbol->getNextSymbol() == NULL)
				{
					if (CurrentSymbol->Terminal() == false && CurrentSymbol->Epsilon() == false)
					{
						//add Follow Set of CurrentNonTermianl to FollowSet of CurrentSymbol
						addFollowToFollow(CurrentNonTerminal, CurrentSymbol);
					}
				}

				CurrentSymbol = CurrentSymbol->getNextSymbol();
			}
			CurrentGrammerRule = CurrentGrammerRule->getNextGrammerRule();
		}
	}
}

void addFollowToFollow(Symbol *First, Symbol *Second)
{
	int FollowSetToAddFrom = indexOf(First);
	int FollowSetToAddToo = indexOf(Second);



	Symbol *temp = FollowSet[FollowSetToAddFrom];

	while (temp != NULL)
	{
		if (temp->Epsilon() != true)
		{
			if (isDuplicateForFollow(temp->getContent(), FollowSetToAddToo) == false)
				addToFollowSet(temp, FollowSetToAddToo);
		}

		temp = temp->getNextSymbol();
	}
}


void wasVisitedToFalse()
{
	for (int i = 0; i < listLength; i++)
	{
		Grammer[i]->setWasVisited(false);
	}
}
