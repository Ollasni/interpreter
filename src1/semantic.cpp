
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include "headers/const.h"
#include "headers/lexem.h"

extern std::map<std::string, int> labels; 
extern std::map<std::string, int> VarTable;
extern std::map<std::string, std::vector<int> > ArrayTable;

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
