#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>

std::map<std::string, int> labels; 
std::map<std::string, int> VarTable;
std::map<std::string, std::vector<int> > ArrayTable;

using namespace std;

enum OPERATOR {
	IF, THEN,
	ELSE, ENDIF,
	WHILE, ENDWHILE,
	GOTO, ASSIGN, COLON,
	LBRACKET, RBRACKET,
	LQBRACKET, RQBRACKET,
	ARRAY,
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
	"if", "then", 
	"else", "endif",
	"while", "endwhile",
	"goto", ":=", ":",
	"(", ")",
	"[", "]",
	"array",
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
	-2, -2,
	-2, -2,
	-2, -2,
	-2, 0 , -2 ,
	1, 1,
	1, 1,
	1,
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
	OPERATORS, NUMBER, VARIABLE, ARRAYS
};

class Lexem {
	LEXEM_TYPE lexem_type;
public:
	Lexem();
	virtual ~Lexem();
	void setType(LEXEM_TYPE);
	LEXEM_TYPE getLexType();
};

Lexem::Lexem() {
}

Lexem::~Lexem() {
}

LEXEM_TYPE Lexem::getLexType() {
	return lexem_type;
}

void Lexem::setType(LEXEM_TYPE lexem) {
	lexem_type = lexem;
}

class Item:public Lexem {
public:
	int virtual getValue() = 0;
};

class Variable: public Item {
	std::string name;
public:
	Variable(const string &name);
	int getValue();
	int setValueTable(int value);
	int setValueLabels(int value);
	string getName();
	int incLabel();
	int getValueLabels();


};


string Variable::getName() {
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



class Number: public Item {
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

class ArrayElem: public Item {
	string name;
	int index;
public:
	ArrayElem(string name1, int index1);
	int getValue();
	int setValue(int);
};

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

int Variable::incLabel() {

	return labels[name];
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

void deleteVector(std::vector <Number *> vec) {
	for(int i = 0; i < vec.size(); i++) {
		if(vec[i] != nullptr)
			delete vec[i];
	}
}
void deleteVector(std::vector <ArrayElem *> vec) {
	for(int i = 0; i < vec.size(); i++) {
		if(vec[i] != nullptr)
			delete vec[i];
	}
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

int evaluatePoliz(std::vector<Lexem *> poliz, int row) {
	stack<Lexem *> stack;
	vector <Number *> VectDelete;
	vector <ArrayElem *>VectDelete2;
	for(int i = 0; i < poliz.size(); i++) {
		if(poliz[i]->getLexType() == NUMBER || poliz[i]->getLexType() == VARIABLE) {
				stack.push(poliz[i]);
				continue;
		}
		if(poliz[i]->getLexType() == OPERATORS) {
			Oper *lexem = (Oper *)poliz[i];
			Goto *lexemGoto = (Goto *)poliz[i];
			if(lexem->getType() == IF || lexem->getType() == WHILE) {
				Lexem *rvalue = stack.top();
				stack.pop();
				int ress = ((Number *)rvalue)->getValue();
				cout << "while cond " << ress << " ";
				if(!ress) {
					deleteVector(VectDelete);
					deleteVector(VectDelete2);
					return (lexemGoto->getRow());
				}
				continue; 
			}
			if(lexem->getType() == ELSE || lexem->getType() == ENDIF || lexem->getType() == ENDWHILE) {
				deleteVector(VectDelete);
				deleteVector(VectDelete2);
				return (lexemGoto->getRow());
			}
			if(lexem->getType() == GOTO) {
				deleteVector(VectDelete);
				deleteVector(VectDelete2);
				return ((Variable *)poliz[i - 1])->getValueLabels();
			}
			if(lexem->getType() == ARRAY || lexem->getType() == LQBRACKET) {
				ArrayElem *elemArr = nullptr;
				Lexem* rightPtr = stack.top();
				stack.pop();
				Lexem *leftPtr = stack.top();
				stack.pop();
				if(lexem->getType() == ARRAY) {
					ArrayTable[((Variable *)leftPtr)->getName()] = vector<int>(((Item *)rightPtr)->getValue(), 0);
				}
				if(lexem->getType() == LQBRACKET) {
					elemArr = new ArrayElem(((Variable *)leftPtr)->getName(), ((Item *)rightPtr)->getValue());
					stack.push(elemArr);
					VectDelete2.push_back(elemArr);
				}
				continue;
			}	
			Lexem *right = (Lexem *)stack.top();
			stack.pop();
			Lexem *left = (Lexem *)stack.top();
			stack.pop();
			Oper *oper = dynamic_cast<Oper *>(poliz[i]);
			Number *ans = oper->getValue(left, right);
			stack.push(ans);
			VectDelete.push_back(ans);
			cout << "answer: " << ans->getValue() << " ";
			continue;
		}
	}
	while(!stack.empty()) {
		stack.pop();
	}
	deleteVector(VectDelete);
	deleteVector(VectDelete2);
	return row + 1;
}

bool isGoto(OPERATOR op) {
	if(op == IF || op == THEN || op == ELSE || op == ENDIF || op == WHILE || op == ENDWHILE || op == GOTO)
		return true;
	return false;
}

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


vector<Lexem *> buildPoliz(std::vector <Lexem *> infix) {
	vector<Lexem *> postfix;
	stack <Oper *> stack;
	for(int i = 0; i < infix.size(); i++) {

		if(infix[i] == nullptr)
			continue;
		if(infix[i]->getLexType() == VARIABLE) {
			postfix.push_back(infix[i]);
		}
		if(infix[i]->getLexType() == NUMBER) {
			postfix.push_back(infix[i]);
			continue;
		}
		if(infix[i] ->getLexType() == OPERATORS) {
			Oper *infixResO = dynamic_cast<Oper *>(infix[i]);
			int resO = infixResO ->getType();
			if(resO == OPERATOR(THEN))
				continue;
			if(resO == OPERATOR(LBRACKET) || resO == OPERATOR(LQBRACKET)) {
				stack.push(infixResO);
				continue;
			}
			if(resO == OPERATOR(RBRACKET)) {
				cout << "false" << endl;
				while((stack.top())->getType() != LBRACKET && !stack.empty()) {
					postfix.push_back(stack.top());
					stack.pop();
				}
				if((stack.top())->getType() == LBRACKET) {
					stack.pop();
				}
			}
			if(resO == OPERATOR(RQBRACKET)) {
				while(((stack.top())->getType() != LQBRACKET) && !stack.empty()) {
					postfix.push_back(stack.top());
					stack.pop();
				}
				if((stack.top())->getType() == LQBRACKET) {
					postfix.push_back(stack.top());
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


void deleteVector(std::vector <Lexem *> vec) {
	for(int i = 0; i < vec.size(); i++) {
		if(vec[i] != nullptr)
			delete vec[i];
	}
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
        cout << endl;

	for (auto x : VarTable) {
		cout << "VarTable" << x.first << " " << x.second << "\n";
	}

	initLabels(infixLines);
	initJumps(infixLines);

	for (auto x : labels) {
		cout << "label " <<  x.first << " " << x.second << "\n";
	}
	if(labels.empty()) {
		cout << "labels empty" << endl;
	}
	
	if(VarTable.empty()) {
		cout << "VarTable empty" << endl;
	}
	
	for(const auto &infix: infixLines)
		postfixLines.push_back(buildPoliz(infix));
	for(int i = 0; i < infixLines.size(); i++) {
		cout << i << ": ";
		print_universal(postfixLines[i]);
	}
	cout << endl;
	int flag = 0;
	int row = 0;
	while(0 <= row && row < postfixLines.size()) {
		row = evaluatePoliz(postfixLines[row], row);
		cout << "row " << row << endl;
		cout << endl;
	}
	for (auto x : VarTable) {
		cout << "VarTable " << x.first << " " << x.second << "\n";
	}
	 for(int i = 0; i < infixLines.size(); i++) {
                deleteVector(infixLines[i]);
        }

	return 0;
}