
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
