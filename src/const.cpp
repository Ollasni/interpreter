#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include "headers/const.h"
#include "headers/lexem.h"

using namespace std;

std::map<std::string, int> labels; 
std::map<std::string, int> VarTable;
std::map<std::string, std::vector<int> > ArrayTable;

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
