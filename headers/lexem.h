
#include <map>

using namespace std;

extern std::map<std::string, int> labels; 
extern std::map<std::string, int> VarTable;
extern std::map<std::string, std::vector<int> > ArrayTable;


class Lexem {
	LEXEM_TYPE lexem_type;
public:
	Lexem();
	virtual ~Lexem();
	void setType(LEXEM_TYPE);
	LEXEM_TYPE getLexType();
};

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

class Number: public Item {
	int value;
public:
	Number(int value);
	int getValue();
};

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

	int setValue(int);
};

