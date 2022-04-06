#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

std::map<std::string, int> labels;
std::map<std::string, int> table;

using namespace std;

enum OPERATOR {
	GOTO, ASSIGN, COLON,
	LBRACKET, RBRACKET,
	OR,
	AND,
	BITTOR,
	XOR,
	BITAND,
	EQ, NEQ,
	LEQ, GEQ,
	SHL, SHR,
	LT, GT,
	PLUS, MINUS,
	MULTIPLY, DIV, MOD
};

string OPERTEXT[] = {
	"goto", ":=", ":",
	"(", ")",
	"or",
	"and",
	"|",
	"^",
	"&",
	"==", "!=",
	"<=", ">=",
	"<<", ">>",
	"<", ">",
	"+", "-",
	"*", "/", "%"
};

int PRIORITY [] = {
	-1 , -1 ,
	0,
	1, 1,
	2,
	3,
	4,
	5,
	6, 6, 
	7, 7,
	8, 8, 
	7, 7,
	9, 9,
	10, 10, 10
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
	bool inLabelTable();
};


string Variable::getName() {
	return name;
}

int Variable::setValue(int value) {
	return table[name] = value;
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
			if(leftPtr == nullptr) {
				exit(1);
			}
				answer = leftPtr->setValue(rightValue);
			break;
	}
	return new Number(answer);
}

class Goto: public Oper {
	int row;
public:
	enum {UNDEFINED = -INT32_MAX };
	Goto(OPERATOR opertype): Oper(opertype) {
		row = UNDEFINED;
	}
	int getRow();
	void setRow(int row);
	void print();

};

void Goto::print() {
	std::cout << "[<row " << row << ">" << OPERTEXT[this->getType()] << "] ";
}

void Goto::setRow(int row) {
	Goto::row = row;
}
int Goto::getRow() {
	return row;
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
	int n = sizeof(OPERTEXT) / sizeof(string);
	for(int j = 0; j < n; j++) {
		string subcodeline = codeline.substr(*ind, OPERTEXT[j].size());
		if(OPERTEXT[j] == subcodeline) {
			*ind += (OPERTEXT[j].size() - 1);
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

void joinGotoAndLabel(Variable *lexemvar, std::stack<Oper *> &stack) {
	if(stack.top()->getType() == GOTO) {
		Goto *lexemgoto = (Goto *)stack.top();
		lexemgoto->setRow(labels[lexemvar->getName()]);
	}
}

bool Variable::inLabelTable() {
	if(labels.find(name) != labels.end()) {
		cout << "found " << endl;
		return true;
	}
	return false;
}



vector<Lexem *> buildPoliz(std::vector <Lexem *> infix) {
	vector<Lexem *> postfix;
	stack <Oper *> stack;
	for(int i = 0; i < infix.size(); i++) {
		if(infix[i] == nullptr)
			continue;
		if(infix[i]->getLexType() == VARIABLE) {
			Variable *lexemvar = (Variable *)infix[i];
			if(lexemvar->inLabelTable() && !stack.empty()) {
				joinGotoAndLabel(lexemvar, stack);
			}
			else
				postfix.push_back(infix[i]);
		}
		if(infix[i]->getLexType() == NUMBER) {
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
				while((stack.top())->getType() != LBRACKET && !stack.empty()) {
					postfix.push_back(stack.top());
					stack.pop();
				}
				if((stack.top())->getType() == LBRACKET) {
					stack.pop();
				}
			}
			else {
				if(!stack.empty()) {
					if((stack.top())->getPriority() >= ((Oper *)infix[i])->getPriority()) {
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

void initLabels(vector<Lexem *> &infix, int row) {
	for (int i = 1; i < infix.size(); i++) {
		if ((infix[i - 1] != nullptr) && (infix[i] != nullptr) &&
			(infix[i - 1]->getLexType() == VARIABLE) && (infix[i]->getLexType() == OPERATORS)) {
			Variable *lexemvar = (Variable *)infix[i - 1];
			Oper *lexemop = (Oper *)infix[i];
			if (lexemop->getType() == COLON) {
				labels[lexemvar->getName()] = row;
				delete infix[i];
				delete infix[i - 1];
				infix[i - 1] = nullptr; 
				infix[i] = nullptr;
				i++;
			}
		 }
	}
}



void print_universal(std::vector < Lexem *> vect) {
	for(int i = 0; i < vect.size(); i++) {
		if(vect[i] == nullptr) {
			//cout << "[00]";
			continue;
		}
		if(vect[i]->getLexType() == NUMBER) {
			Number *num = dynamic_cast<Number *> (vect[i]);
			cout << "[" << (num->getValue()) << "]";
		}
		if(vect[i]->getLexType() == OPERATORS) {
         	Oper *oper = dynamic_cast<Oper *>(vect[i]);
			OPERATOR val = oper->getType();
			cout << "[" << OPERTEXT[val] << "]";
		}
		if(vect[i]->getLexType() == VARIABLE) {
			Variable *var = dynamic_cast<Variable *>(vect[i]);
			cout <<  "[" << var ->getName() << "=" << var->getValue() << "]";
		}
		
	}
	cout << endl;
	return;
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
			cout << OPERTEXT[val] << " ";
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
			cout << OPERTEXT[val] << " ";
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
	std::vector< std::vector<Lexem *> > infixLines, postfixLines;
	int value;
	while(std::getline(std::cin, codeline))
		infixLines.push_back(parseLexem(codeline));
	for(int i = 0; i < infixLines.size(); i++) {
		cout << i << ": ";
		print_universal(infixLines[i]);
	}
	for (auto x : table) {
		cout << x.first << " " << x.second << "\n";
	}
	for(int row = 0; row < infixLines.size(); ++row) {
		initLabels(infixLines[row], row);
	}
	for (auto x : labels) {
		cout << x.first << " " << x.second << "\n";
	}
	
	for(const auto &infix: infixLines)
		postfixLines.push_back(buildPoliz(infix));
	for(int i = 0; i < infixLines.size(); i++) {
		cout << i << ": ";
		print_universal(postfixLines[i]);
	}
	int row = 0;
	cout << "t" << endl;
	//while(0 <= row && row < postfixLines.size())
	//	row = evaluatePoliz(postfixLines[row], row);
	
	//getline(std::cin, codeline);
	/*while(std::getline(std::cin, codeline)) {
		infix = parseLexem(codeline);
		print1(infix);
		//cout << infix.size() << endl;
		postfix = buildPoliz(infix);
		cout << "done" << endl;
		print1(postfix);
		value = evaluatePoliz(postfix);
		infix.clear();
		std::cout << value << std::endl;
	}*/
	return 0;
}