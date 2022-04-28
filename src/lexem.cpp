#include <iostream>
#include "headers/const.h"
#include "headers/lexem.h"

extern std::map<std::string, int> labels; 
extern std::map<std::string, int> VarTable;
extern std::map<std::string, std::vector<int> > ArrayTable;


Lexem::Lexem() {}

Lexem::~Lexem() {}

LEXEM_TYPE Lexem::getLexType() {
	return lexem_type;
}

void Lexem::setType(LEXEM_TYPE lexem) {
	lexem_type = lexem;
}

tring Variable::getName() {
	return name;
}

int Variable::setValueTable(int value) {
	return VarTable[name] = value;
}

int Variable::setValueLabels(int value) {
	return labels[name] = value;
}
int Variable::getValueLabels() {
	return labels[name];
}

int Variable::getValue() {
	return VarTable[name];
}

Variable::Variable(const string &name){
	Variable::name = name;
	VarTable[name] = 0;
	this->setType(VARIABLE);
}

int Variable::incLabel() {

	return labels[name];
}


Number::Number(int value) {
	Number::value = value;
	this->setType(NUMBER);
}

int Number::getValue() {
	return value;
}

OPERATOR Oper::getType() {
	return opertype;
}

int Oper::getPriority() {
	return PRIORITY[getType()];
}

Oper::Oper(OPERATOR opertype) {
	Oper::opertype = opertype;
	this->setType(OPERATORS);
}

Oper::Oper(int ind) {
	opertype = (OPERATOR)ind;
	this->setType(OPERATORS);
}

Number* Oper::getValue(Lexem *left, Lexem *right) {
	int leftValue = 0, rightValue = 0;
	Variable *leftPtr = (Variable *)left;
	ArrayElem *leftPtr2 = (ArrayElem *)left;
	leftValue = ((Item *)left)->getValue();
	rightValue = ((Item *)right)->getValue();
	int answer = 0;
	switch(getType()) {
		case OR:
			answer = leftValue or rightValue;
			break;
		case AND:
			answer = leftValue and rightValue;
			break;
		case BITTOR:
			answer = leftValue | rightValue;
			break;
		case XOR:
			answer = leftValue ^ rightValue;
			break;
		case BITAND:
			answer = leftValue & rightValue;
			break;
		case EQ:
			answer = leftValue == rightValue;
			break;
		case NEQ:
			answer = leftValue != rightValue;
			break;
		case LEQ:
			answer = leftValue <= rightValue;
			break;
		case LT:
			answer = leftValue < rightValue;
			break;
		case GEQ:
			answer = leftValue >= rightValue;
			break;
		case GT:
			answer = leftValue > rightValue;
			break;
		case SHL:
			answer = leftValue << rightValue;
			break;
		case SHR:
			answer = leftValue >> rightValue;
			break;
		case DIV:
			answer = leftValue / rightValue;
			break;
		case PLUS:
			answer = leftValue + rightValue;
			break;
		case MINUS:
			answer = leftValue - rightValue;
			break;
		case MULTIPLY:
			answer = leftValue * rightValue;
			break;
		case ASSIGN:
			if(leftPtr == nullptr && leftPtr2 == nullptr) {
				cout << "err ASSIGN" << endl;
				exit(1);
			}
				if(left->getLexType() == VARIABLE) {
					answer =leftPtr->setValueTable(rightValue);
					cout << "setVarTable" << answer  << endl;
				}
				if(left->getLexType() == ARRAYS) {
					answer = leftPtr2->setValue(rightValue);
					cout << "setTable" << answer  << endl;
				}
			break;
	}
	return new Number(answer);
}



void Goto::print() {
	std::cout << "[<row " << row << ">" << OPERTEXT[this->getType()] << "] ";
}

void Goto::setRow(int row) {
	Goto::row = row;
}
int Goto::getRow() {
	return row;
}

ArrayElem::ArrayElem(string name1, int index1) {
	name = name1;
	index = index1;
	this->setType(ARRAYS);
}


int ArrayElem::getValue() {
	return ArrayTable[name][index];
}

int ArrayElem::setValue(int number) {
	return ArrayTable[name][index] = number;
}

vector<Lexem *> parseLexem (string codeline) {
	vector<Lexem *> infix;
	int i;
	for(i = 0; i < codeline.size(); i++) {
		Oper *ptrO = checkOperator(codeline, &i);
		if(ptrO) {
			infix.push_back(ptrO);
			continue;
		}
		Number *ptrN = checkNumber(codeline, &i);
		if(ptrN) {
			infix.push_back(ptrN);
			i--;
			continue;
		}
		Variable *ptrV = checkVariable(codeline, &i);
		if(ptrV) {
			infix.push_back(ptrV);
			i--;
			continue;
		}
		if(codeline[i] == ' ' || codeline[i] == '\t')
			continue;
		cout << "err"  << i << endl;

	}
	return infix;
}
