#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>


std::map<std::string, int> table;

using namespace std;

enum OPERATOR {
	LBRACKET, RBRACKET,
	ASSIGN,
	PLUS, MINUS,
	MULTIPLY
};

char OPERATOR_STRING[] = {
	'(', ')',
	'=',
	'+', '-',
	'*'
};

int PRIORITY [] = {
	-1 , -1 ,
	0,
	1, 1,
	2
};

enum LEXEM_TYPE {
	OPERATORS, NUMBER, VARIABLE
};

class Lexem {
	LEXEM_TYPE lexem_type;
public:
	Lexem();
	virtual ~Lexem() {}
	void setType(LEXEM_TYPE);
	LEXEM_TYPE getLexType();
	void print(vector <Lexem *>);
};

LEXEM_TYPE Lexem::getLexType() {
	return lexem_type;
}

void Lexem::setType(LEXEM_TYPE lexem) {
	lexem_type = lexem;
}

Lexem::Lexem() {}



class Variable: public Lexem{
	std::string name;
public:
	Variable(const string &name);
	int getValue();
	int setValue(int value);
	string getName();
};


string Variable::getName() {
	return name;
}

int Variable::setValue(int value) {
	table[name] = value;
	cout << "map: " << table[name] << endl;
	return table[name];
}

int Variable::getValue() {
	return table[name];
}

Variable::Variable(const string &name){
	Variable::name = name;
	this->setType(VARIABLE);
}



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
	Number *getValue(Lexem *left,
		     Lexem *right);
};

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
	Number *leftCasted = nullptr, *rightCasted = nullptr;
	int leftValue = 0, rightValue = 0;
	Variable *leftPtr = nullptr, *rightPtr = nullptr;
	if(left->getLexType() == NUMBER) {
		leftCasted = dynamic_cast<Number *>(left);
		leftValue = leftCasted->getValue();
	}
	if(right->getLexType() == NUMBER) {
		rightCasted = dynamic_cast<Number *>(right);
		rightValue = rightCasted->getValue();
	}
	if(left->getLexType() == VARIABLE) {
		leftPtr = dynamic_cast<Variable *> (left);
		leftValue = table[leftPtr->getName()];
	}
	if(right->getLexType() == VARIABLE) {
        rightPtr = dynamic_cast<Variable *> (right);
		rightValue = table[rightPtr->getName()];
        }

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
		case ASSIGN:
			cout << "yes" << endl;
			if(leftPtr == nullptr) {
				exit(1);
			}
				answer = leftPtr->setValue(rightValue);
			break;
	}
	return new Number(answer);
}

int evaluatePoliz(std::vector<Lexem *> poliz) {
	Number* answer(0);
	stack<Lexem *> stack;
	for(int i = 0; i < poliz.size(); i++) {
			if(poliz[i]->getLexType() == NUMBER || poliz[i]->getLexType() == VARIABLE) {
				stack.push(poliz[i]);
				continue;
		}
		if(poliz[i]->getLexType() == OPERATORS) {
			Lexem *right = (Lexem *)stack.top();
			stack.pop();
			Lexem *left = (Lexem *)stack.top();
			stack.pop();
			Oper *oper = dynamic_cast<Oper *>(poliz[i]);
			Number *ans = oper->getValue(left, right);
			stack.push(ans);
			continue;
		}
	}
	answer = dynamic_cast<Number* >(stack.top());
	while(!stack.empty()) {
		stack.pop();
	}
	return (answer->getValue());
}


Oper *checkOperator(string codeline, int *ind) {
	for(int j = 0; j < sizeof(OPERATOR_STRING); j++) {
		if(codeline[*ind] == OPERATOR_STRING[j]) {
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
	if(codeline[*ind] >= 'a' && codeline[*ind] <= 'z') {
		while(isdigit(codeline[*ind]) || (codeline[*ind] >= 'a' && codeline[*ind] <= 'z')) {
			name.push_back(codeline[*ind]);
			(*ind)++;
		}
		return new Variable(name);
	}
	else 
		return nullptr;
}

vector<Lexem *> parseLexem (string codeline) {
	vector<Lexem *> infix;
	int i;
	for(i = 0; i < codeline.size(); i++) {
		Variable *ptrV = checkVariable(codeline, &i);
		if(ptrV) {
			infix.push_back(ptrV);
		}

		Oper *ptrO = checkOperator(codeline, &i);
		if(ptrO) {
			infix.push_back(ptrO);
		}
		Number *ptrN = checkNumber(codeline, &i);
		if(ptrN) {
			infix.push_back(ptrN);
		}

	}
	return infix;
}




vector<Lexem *> buildPoliz(std::vector <Lexem *> infix) {
	vector<Lexem *> postfix;
	stack <Lexem *> stack;
	for(int i = 0; i < infix.size(); i++) {
		if(infix[i]->getLexType() == NUMBER ||infix[i]->getLexType() == VARIABLE) {
			postfix.push_back(infix[i]);
			continue;
		}
		if(infix[i] ->getLexType() == OPERATORS) {
			Oper *infixResO = dynamic_cast<Oper *>(infix[i]);
			int resO = infixResO ->getType();
			if(resO == OPERATOR(LBRACKET)) {
				stack.push(infixResO);
				continue;
			}
			if(resO == OPERATOR(RBRACKET)) {
				while(((Oper *)stack.top())->getType() != LBRACKET && !stack.empty()) {
					postfix.push_back(stack.top());
					stack.pop();
				}
				if(((Oper *)stack.top())->getType() == LBRACKET) {
					stack.pop();
				}
			}
			else {
				if(!stack.empty()) {
					if(((Oper *)stack.top())->getPriority() >= ((Oper *)infix[i])->getPriority()) {
						postfix.push_back(stack.top());
						stack.pop();
					}
				}
				stack.push((Oper *)infix[i]);
			}
		}
	}

	for(int i = stack.size(); i > 0; i--) {
		postfix.push_back(stack.top());
		stack.pop();
	}
	return postfix;
}


void Lexem::print(std::vector <Lexem *> vec) {
	for(int i = 0; i < vec.size(); i++) {
		if(vec[i]->getLexType() == NUMBER) {
			Number *num = dynamic_cast<Number *> (vec[i]);
			cout << (num->getValue()) << " ";
		}
		if(vec[i]->getLexType() == OPERATORS) {
	         	Oper *oper = dynamic_cast<Oper *>(vec[i]);
			OPERATOR val = oper->getType();
			cout << OPERATOR_STRING[val] << " ";
		}
	}
	cout << endl;
	return;
}

void print1(std::vector <Lexem *> vec) {
	for(int i = 0; i < vec.size(); i++) {
		if(vec[i]->getLexType() == NUMBER) {
			Number *num = dynamic_cast<Number *> (vec[i]);
			cout << (num->getValue()) << " ";
		}
		if(vec[i]->getLexType() == OPERATORS) {
         	Oper *oper = dynamic_cast<Oper *>(vec[i]);
			OPERATOR val = oper->getType();
			cout << OPERATOR_STRING[val] << " ";
		}
		if(vec[i]->getLexType() == VARIABLE) {
			Variable *var = dynamic_cast<Variable *>(vec[i]);
			cout << var ->getName() << " ";
		}
	}
	cout << endl;
	return;
}

void free(std::vector <Lexem *> vec) {
	for(int i = 0; i < vec.size(); i++) {
		delete vec[i];
	}
	cout << "free" << endl;
	return;
}



int main() {

	string codeline;
	vector<Lexem *> infix;
	vector<Lexem *> postfix;
	int value;
//	getline(std::cin, codeline);
	while(std::getline(std::cin, codeline)) {
		infix = parseLexem(codeline);
		print1(infix);
		//cout << infix.size() << endl;
		postfix = buildPoliz(infix);
		cout << "done" << endl;
		print1(postfix);
		value = evaluatePoliz(postfix);
		infix.clear();
		std::cout << value << std::endl;
	}
	return 0;
}
