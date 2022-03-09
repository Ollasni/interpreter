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

int PRIORITY [] = {
	-1 , -1 ,
	0, 0,
	1
};

enum LEXEM_TYPE {
	OPERATOR, NUMBER
};

class Lexem {
	LEXEM_TYPE lexem_type;
public:
	Lexem();
};

Lexem::Lexem() {};

class Number: public Lexem {
	int value;
public:
	Number(int value);
	int getValue();
};

Number::Number(int value) {
	Number::value = value;
}

int Number::getValue() {
	return value;
}

class Oper: public Lexem {
	OPERATOR opertype;
public:
	Oper(OPERATOR opertype);
	OPERATOR getType();
	int getPriority();
	int getValue(const Number & left, 
		     const Number & right);
};

int Oper::getValue(const Number & left, const Number & right) {
	int answer = 0;
	switch(getType()) {
		case PLUS:
			answer = left + right;
			break;
		case MINUS:
			answer = left - right;
			break;
		case MULTIPLY:
			answer = left * right;
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
}

vector<Lexem *> parseLexem (string codeline) {
	vector<Lexem *> infix;
	for(int i = 0; i < codeline.size(); i++) {
		int number = 0;
		if(isdigit(codeline[i])) {
			while(isdigit(codeline[i])) {
				number = 10 * number + codeline[i] - '0';
				i++;
			}
			infix.push_back(new Number(number));
		}
		if(codeline[i] == '+') {
			infix.push_back(new Oper(PLUS));
			continue;
		}
		if(codeline[i] == '-') {
			infix.push_back(new Oper(MINUS));
			continue;
		}
		if(codeline[i] == '*') {
			infix.push_back(new Oper(MULTIPLY));
			continue;
		}
		if(codeline[i] == '(') {
			infix.push_back(new Oper(LBRACKET));
			continue;
		}
		if(codeline[i] == ')') {
			infix.push_back(new Oper(RBRACKET));
		}
		number = 0;
	}
	return infix;
}

int evaluatePoliz(std::vector<Lexem *> poliz) {
	int answer = 0;
	stack<int> stack;
	for(int i = 0; poliz.size(); i++) {
		if(poliz[i] == NUMBER) {
			stack.push(poliz[i]);
		}
		if(poliz[i] == OPERATOR) {
			int right = stack.top();
			int left = stack.top();
			stack.push(getValue(left, right));
		}
	}
	answer = stack.top();
	return answer;
}

vector<Lexem *> buildPoliz(vector <Lexem *> infix) {
	vector<Lexem *> postfix;
	stack <int> stack;
	for(int i = 0; i < infix.size(); i++) {
		if(isdigit(infix[i])) {
			postfix.push_back(new infix[i]);
			continue;
		}
		if(infix[i] == OPERATOR(LBRACKET)) {
			stack.push(Oper(LBRACKET));
			break;
		}
		if(infix[i] == OPERATOR(RBRACKET)) {
			int x = 100;
			while(1) {
				x = stack.top();
				if(x == OPERATOR(LBRACKET))
					break;
				postfix.push_back(new OPERATOR[x]);
			}
		}
		if(infix[i] == OPERATOR(MINUS) && infix[i] == OPERATOR(PLUS) && infix[i] == OPERATOR(MULTIPLY) {
			int x = stack.top();
			if(x.getPriority() >= infix[i].getPriority()) {
				postfix.push_back(new Oper(OPERATOR[x]));
			}
		stack.push(infix[i]);
		}
	}
	while(stack) {
		int x = stack.top();
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
