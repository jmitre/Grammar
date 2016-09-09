#ifndef __SYMBOLS_H_
#define __SYMBOLS_H_

#include <string>

using namespace std;




class Symbol
{
private:
	Symbol *nextSymbol;
	Symbol *nextGrammer;
	string content;
	bool isEpsilon;
	bool isTerminal;
	bool wasVisited;
public:
	Symbol(string Content,bool isTerm,bool isEp)
	{
		content = Content;
		nextSymbol = NULL;
		nextGrammer = NULL;
		isTerminal = isTerm;
		isEpsilon = isEp;
		wasVisited = false;
	}

	void setNextSymbol(Symbol *nextS)
	{
		nextSymbol = nextS;
	}

	void setNextGrammer(Symbol *nextg)
	{
		nextGrammer = nextg;
	}
	string getContent()
	{
		return content;
	}
	Symbol* getNextSymbol()
	{
		return nextSymbol;
	}
	Symbol *getNextGrammerRule()
	{
		return nextGrammer;
	}
	bool Epsilon()
	{
		return isEpsilon;
	}
	bool Terminal()
	{
		return isTerminal;
	}
	bool getwasVisited()
	{
		return wasVisited;
	}
	void setWasVisited(bool b)
	{
		wasVisited = b;
	}
};

class Terminal : public Symbol
{
public:
	Terminal(string Content) : Symbol(Content,true,false){}
};

class NonTerminal : public Symbol
{
private:
	NonTerminal *mySymbol;
	NonTerminal *next;
	bool hasEps;
public:
	NonTerminal(string content) :Symbol(content,false,false)
	{
		mySymbol = NULL;
		hasEps = false;
	}
	void setMySymbol(NonTerminal *mysym)
	{
		mySymbol = mysym;
	}
	NonTerminal* getMySymbol()
	{
		return mySymbol;
	}
	NonTerminal* getNext()
	{
		return next;

	}
	void setNext(NonTerminal *Next)
	{
		next = Next;
	}
	bool hasEpsilon()
	{
		return hasEps;
	}
	void setHasEpsilon(bool b)
	{
		hasEps = b;
	}
};

class Epsilon :public Symbol
{
public:
	Epsilon():Symbol("#", false, true){}
	
};

class EndOfFile : public Symbol
{
public:
	EndOfFile() :Symbol("$", false, false){}
};


#endif