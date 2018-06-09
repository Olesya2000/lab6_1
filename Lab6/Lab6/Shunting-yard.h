#ifndef _SHUNTING_YARD_H_
#define _SHUNTING_YARD_H_

#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include <map>
#include <cstdlib>
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
double calculator(string, double, double);
bool is_operator(string);


#endif
