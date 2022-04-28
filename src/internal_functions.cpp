
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include "headers/const.h"
#include "headers/lexem.h"


Oper *checkOperator(string codeline, int *ind) {
	int n = sizeof(OPERTEXT) / sizeof(string);
	for(int j = 0; j < n; j++) {
		string subcodeline = codeline.substr(*ind, OPERTEXT[j].size());
		if(OPERTEXT[j] == subcodeline) {
			*ind += (OPERTEXT[j].size() - 1);
			if(isGoto((OPERATOR)j)) {
				return new Goto((OPERATOR)j);
			}
			return new Oper((OPERATOR)j);
		}
	}
	return nullptr;
}

Number *checkNumber(string codeline, int *ind) {
	int number = 0;
		if(isdigit(codeline[*ind])) {
			while(isdigit(codeline[*ind])) {
				number = 10 * number + (codeline[*ind] - '0');
				(*ind)++;
			}
		return new Number(number);
	}
	return nullptr;
}

Variable *checkVariable(string codeline, int *ind) {
	string name;
	if((codeline[*ind] >= 'a' && codeline[*ind] <= 'z') || (codeline[*ind] >= 'A' && codeline[*ind] <= 'Z')) {
		while(isdigit(codeline[*ind]) || (codeline[*ind] >= 'a' && codeline[*ind] <= 'z')|| (codeline[*ind] >= 'A' && codeline[*ind] <= 'Z')) {
			name.push_back(codeline[*ind]);
			(*ind)++;
		}
		return new Variable(name);
	}
	else 
		return nullptr;
}

void initLabels(std::vector < vector<Lexem *> > &infix) {
	int flag = 0;
	for (int row = 0; row  < infix.size(); row++) {
		for (int i = 1; i < infix[row].size(); i++) {
			if ((infix[row][i - 1] != nullptr) && (infix[row][i] != nullptr) &&
				(infix[row][i - 1]->getLexType() == VARIABLE) && (infix[row][i]->getLexType() == OPERATORS)) {
				Variable *lexemvar = (Variable *)infix[row][i - 1];
				Oper *lexemop = (Oper *)infix[row][i];
				if (lexemop->getType() == COLON) {
					labels[lexemvar->getName()] = row;
					delete infix[row][i];
					delete infix[row][i - 1];
					infix[row][i - 1] = nullptr; 
					infix[row][i] = nullptr;
					i++;
				}
			 }

		}
	}
}

void initJumps(std::vector < vector<Lexem *> > &infix) {
	std::stack<Goto *> stackIfElse;
	std::stack<Goto *> stackWhile;
	for (int row = 0; row < infix.size(); row++) {
		for(int i = 0; i < (int)infix[row].size(); i++) {
			if(infix[row][i]->getLexType() == OPERATORS) {
				Oper *lexemOper = (Oper *)infix[row][i];
				Goto *lexemGoto = (Goto *)lexemOper;
				switch(lexemOper->getType()) {
					case IF:
						stackIfElse.push(lexemGoto);
						break;
					case ELSE:
						stackIfElse.top()->setRow(row + 1);
						stackIfElse.pop();
						stackIfElse.push(lexemGoto);
						break;
					case ENDIF:
						lexemGoto->setRow(row + 1);
						stackIfElse.top()->setRow(row + 1);
						stackIfElse.pop();
						break;
					case WHILE:
						lexemGoto->setRow(row);
						stackWhile.push(lexemGoto);
						break;
					case ENDWHILE:
						lexemGoto->setRow(stackWhile.top()->getRow());
						stackWhile.top()->setRow(row + 1);
						stackWhile.pop();
						break;

				}
			}
		}
	}
	while(!stackIfElse.empty()) {
		cout << "err init jump " << endl;
		stackIfElse.pop();
	}
}
