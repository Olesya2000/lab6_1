#ifndef _SHUNTING_YARD_H_
#define _SHUNTING_YARD_H_

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <map>
#include <sstream>

using namespace std;

struct Node
{
	string val;
	vector<Node*> sons;
};
struct Stack
{
	Node* data;
	Stack* next;
};
string delete_space(string s);
void print_postf_bin(Node* root);
void print_inf_bin(Node* root);
void push_str(Stack**, string);
void push_node(Stack** s, Node** node);
string getFront(Stack*);
Node* pop(Stack**);
Node* calc(string);
string getOp(char s);
int getPriority(string op);
double calculator(string op, double a, double b);
bool is_number(string s);
string find_rez(Node* ast, map <string, double>& mymap);
string optimization(Node* ast, map <string, double>& mymap);
bool is_zero(string s);
bool is_operator(string s);

#endif
