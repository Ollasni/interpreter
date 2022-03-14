#include <iostream>
#include <string>
#include <vector>
#include <stack>

using namespace std;

enum OPERATOR {
	LBRACKET, RBRACKET,
	PLUS, MINUS,
	MULTIPLY
};

char OPERATOR_STRING[] = {
	'(', ')',
	'+', '-',
	'*'
};

int PRIORITY [] = {
	-1 , -1 ,
	0, 0,
	1
};

enum LEXEM_TYPE {
	OPERATORS, NUMBER
};

class Lexem {
	LEXEM_TYPE lexem_type;
public:
	Lexem();
	virtual ~Lexem();
	void setType(LEXEM_TYPE);
	LEXEM_TYPE getLexType() {
		return lexem_type;
	}
};

void Lexem::setType(LEXEM_TYPE lexem) {
	lexem_type = lexem;
}

Lexem::Lexem() {};

class Number: public Lexem {
	int value;
public:
	Number(int value);
	int getValue();
};

Number::Number(int value) {
	Number::value = value;
	this->setType(NUMBER);
}

int Number::getValue() {
	return value;
}

class Oper: public Lexem {
	OPERATOR opertype;
public:
	Oper(OPERATOR opertype);
	Oper(int);
	OPERATOR getType();
	int getPriority();
	int getValue(Number *left, 
		     Number *right);
};

int Oper::getValue(Number *left, Number *right) {
	int leftValue = left->getValue();
	int rightValue = right->getValue();
	int answer = 0;
	switch(getType()) {
		case PLUS:
			answer = leftValue + rightValue;
			break;
		case MINUS:
			answer = leftValue - rightValue;
			break;
		case MULTIPLY:
			answer = leftValue * rightValue;
			break;
	}
	return answer;
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

Oper *checkOperator(string codeline, int *ind) {
	for(int j = 0; j < sizeof(OPERATOR_STRING); j++)
		if(codeline[*ind] == OPERATOR_STRING[j]) {
			return new Oper((OPERATOR)j);
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

vector<Lexem *> parseLexem (string codeline) {
	vector<Lexem *> infix;
	for(int i = 0; i < codeline.size(); i++) {
		Oper *ptrO = checkOperator(codeline, &i);
		if(ptrO)
				infix.push_back(ptrO);
		Number *ptrN = checkNumber(codeline, &i);
		if(ptrN)
			infix.push_back(ptrN);
	}
	return infix;
}

int evaluatePoliz(std::vector<Lexem *> poliz) {
	Number* answer(0);
	stack<Lexem *> stack;
	for(int i = 0; poliz.size(); i++) {
		if(poliz[i]->getLexType() == NUMBER) {
			stack.push(poliz[i]);
		}
		if(poliz[i]->getLexType() == OPERATORS) {
			Number *right = dynamic_cast<Number *>(stack.top());
			Number *left = dynamic_cast<Number *> (stack.top());
			Oper *res = dynamic_cast<Oper *>(poliz[i]);
			res ->getValue(left, right);
			stack.push(res);
		}
	}
	answer = dynamic_cast<Number* >(stack.top());
	return (answer ->getValue());
}

vector<Lexem *> buildPoliz(std::vector <Lexem *> infix) {
	vector<Lexem *> postfix;
	stack <Lexem *> stack;
	for(int i = 0; i < infix.size(); i++) {
		if(infix[i]->getLexType() == NUMBER) {
			Number *infixResN = dynamic_cast<Number *> (infix[i]);
			int resN = infixResN->getValue();
			postfix.push_back(new Number(resN));
			continue;
		}
		if(infix[i] ->getLexType() == OPERATORS) {
			Oper *infixResO = dynamic_cast<Oper *>(infix[i]);
			int resO = infixResO ->getType();
			
			if(resO == OPERATOR(LBRACKET)) {
				stack.push(infixResO);
				break;
			}
			
			if(resO == OPERATOR(RBRACKET)) {
				while(1) {
					Lexem *xaxa = stack.top();
					Oper *xaxaO = dynamic_cast<Oper *>(xaxa);
					int xaxaRes =xaxaO->getType();
					if(xaxaRes == OPERATOR(LBRACKET))
						break;
					postfix.push_back(new Oper(i));
				}
			}
			if(resO == OPERATOR(MINUS) && resO == OPERATOR(PLUS) && resO == OPERATOR(MULTIPLY)) {
				Lexem *valL = stack.top();
				Oper *valO = dynamic_cast<Oper *>(valL);
				if((valO->getPriority()) >= infixResO->getPriority()) {
					postfix.push_back(new Oper(i));
			}
			stack.push(infix[i]);
			}
		}
	}
	while(1) {
		Lexem *x = stack.top();
		if(x == nullptr)
			break;
		postfix.push_back(x);
	}
	return postfix;
}

int main() {

	string codeline;
	vector<Lexem *> infix;
	vector<Lexem *> postfix;
	int value;

	while(std::getline(std::cin, codeline)) {
		infix = parseLexem(codeline);
		postfix = buildPoliz(infix);
		value = evaluatePoliz(postfix);
		std::cout << value << std::endl;
	}
	return 0;
}
