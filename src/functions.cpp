
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include "headers/const.h"
#include "headers/lexem.h"


void deleteVector(std::vector <ArrayElem *> vec) {
	for(int i = 0; i < vec.size(); i++) {
		if(vec[i] != nullptr)
			delete vec[i];
	}
}

void deleteVector(std::vector <Number *> vec) {
	for(int i = 0; i < vec.size(); i++) {
		if(vec[i] != nullptr)
			delete vec[i];
	}
}

bool isGoto(OPERATOR op) {
	if(op == IF || op == THEN || op == ELSE || op == ENDIF || op == WHILE || op == ENDWHILE || op == GOTO)
		return true;
	return false;
}

void print_universal(std::vector < Lexem *> vect) {
	for(int i = 0; i < vect.size(); i++) {
		if(vect[i] == nullptr) {
				continue;
		}
		if(vect[i]->getLexType() == NUMBER) {
			Number *num = dynamic_cast<Number *> (vect[i]);
			cout << "[" << (num->getValue()) << "]";
		}
		if(vect[i]->getLexType() == OPERATORS) {
			if(	Goto *got = dynamic_cast<Goto *>(vect[i]))
				got->print();
         	Oper *oper = dynamic_cast<Oper *>(vect[i]);
			OPERATOR val = oper->getType();
			cout << "[" << OPERTEXT[val] << "]";
		}
		if(vect[i]->getLexType() == VARIABLE) {
			Variable *var = dynamic_cast<Variable *>(vect[i]);
			cout <<  "[" << var ->getName() << "]";
		}
	}
	cout << endl;
	return;
}


