#ifndef __PARSER_H_
#define __PARSER_H_

#include <iostream>
#include <string.h>
#include "Symbols.h"

using namespace std;

int listLength = 0;
string line = "";
int place = 0;

NonTerminal* parseNonTerminals(string content)
{
	string nameOfSymbol = "";
	NonTerminal *newNTerm;
	NonTerminal *head = NULL;

	int i = 0;
	while (content.at(i) != '#')
	{
		if (content.at(i) != ' ')
			nameOfSymbol += content.at(i);
		else
		if (nameOfSymbol.compare("") != 0 && nameOfSymbol.compare(" ") != 0)
		{
			newNTerm = new NonTerminal(nameOfSymbol);
			if (head == NULL)
			{
				head = newNTerm;
				listLength++;
			}
			else
			{
				newNTerm->setNext(head);
				head = newNTerm;
				listLength++;

			}


			nameOfSymbol = "";
		}



		if (i == content.length() - 1 && content.at(i) != '#')//if a next line character has been reached, but all the non-terminals haven't been read
		{
			if (nameOfSymbol.compare("") != 0 && nameOfSymbol.compare(" ") != 0)
			{
				newNTerm = new NonTerminal(nameOfSymbol);
				if (head == NULL)
				{
					head = newNTerm;
					listLength++;
				}
				else
				{
					newNTerm->setNext(head);
					head = newNTerm;
					listLength++;

				}


				nameOfSymbol = "";
			}


			getline(cin, content);
			i = 0;
		}
		else
			i++;

	}

	return head;
}

NonTerminal* SearchForNonT(string symb, NonTerminal** Grammer)
{
	NonTerminal *NonT = NULL;

	for (int i = 0; i < listLength; i++)
	{
		if (Grammer[i]->getContent().compare(symb) == 0)
		{
			NonT = Grammer[i];
			break;
		}
	}

	return NonT;
}


string getToken()
{
	string token = "";
	bool tokenComplete = false;

	while (line.length() == 0)
		getline(cin, line);


	while (tokenComplete == false)
	{
		while (place == line.size() || line.size() == 0)
		{
			getline(cin, line);
			place = 0;

			if (token.size() != 0)
			{
				return token;
			}
		}



		if (token.size() > 0)
		{
			if (line.at(place) != '#' && token.at(token.size() - 1) == '#')
			{
				return token;
			}
		}
		if (line.at(place) == '#' && token.size() != 0)
		{
			if (token.at(token.size() - 1) != '#')
			{
				//place--;
				return token;
			}

		}
		if (line.at(place) != ' ')
			token = token + line.at(place);
		else
		if (token.length() != 0)
			tokenComplete = true;

		place++;


	}



	return token;
}





void parseGrammer(NonTerminal **Grammer)
{
	string LeftNTermS;
	NonTerminal *LeftNTermP;

	string newSymbol;
	NonTerminal *NewNonTerm;
	NonTerminal *MyNonTerm;
	Epsilon *NewEpsion;
	Terminal *NewTerminal;
	Symbol *Place;

	newSymbol = getToken();
	while (newSymbol.compare("##") != 0)
	{
		//Get NonTerminal then look it up in the array
		LeftNTermP = SearchForNonT(newSymbol,Grammer);
		if(LeftNTermP == NULL){
			cout << "The Non-Terminal "+ newSymbol + " is not definded above the grammer\n";
			exit(1);
		}
		getToken();//gets "->"



		//places us in the correct grammer Rule to add symbols to
		Place = LeftNTermP;
		while (Place->getNextGrammerRule() != NULL)
		{
			Place = Place->getNextGrammerRule();
		}

		newSymbol = getToken();//gets the first right hand side symbol

		if (newSymbol.compare("#")==0)
		{
			//you should insert epsilon.
			NewEpsion = new Epsilon();
			Place->setNextGrammer(NewEpsion);
		}
		else//the case that we are not adding epsilon
		{

			//add to the first Grammer Rule pointer; this symbol will be the first symbol in a grammer rule
			if ((MyNonTerm = SearchForNonT(newSymbol, Grammer)))
			{
				NewNonTerm = new NonTerminal(newSymbol);
				NewNonTerm->setMySymbol(MyNonTerm);
				Place->setNextGrammer(NewNonTerm);
			}
			else//its a terminal
			{
				NewTerminal = new Terminal(newSymbol);
				Place->setNextGrammer(NewTerminal);
			}

			Place = Place->getNextGrammerRule();  //places us in the grammer Rule we just created

			newSymbol = getToken();

			while (newSymbol.compare("#") != 0)
			{
				//see if newSymbol is a nonTerminal
				MyNonTerm = SearchForNonT(newSymbol, Grammer);
				if (MyNonTerm != NULL)
				{
					NewNonTerm = new NonTerminal(newSymbol);
					NewNonTerm->setMySymbol(MyNonTerm);
					Place->setNextSymbol(NewNonTerm);
				}
				else
				{//its a terminal
					NewTerminal = new Terminal(newSymbol);
					Place->setNextSymbol(NewTerminal);
				}

				Place = Place->getNextSymbol();

				newSymbol = getToken();

			}


		}
		newSymbol = getToken();
		//get Corresponding right handside symbols
	}
}




#endif
