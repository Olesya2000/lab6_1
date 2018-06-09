#include "Shunting-yard.h"
double calculator(string op, double a, double b) {
	if (op == "+")
		return a + b;
	if (op == "-")
		return a - b;
	if (op == "*")
		return a * b;
	if (op == "/")
		return a / b;
	if (op == "^")
		return pow(a, b);
}
bool is_number(string s) {
	for (int i = 0; i < s.length(); i++)
		if (!((s[i] >= '0' && s[i] <= '9') || s[i] == '.'))
			return false;
	return true;
}
string find_rez(Node* ast, map <string, double>& mymap) {
	if (ast->val == "<<")
		cout << find_rez(ast->sons[0], mymap) << endl;
	else if (ast->val == "=") {
		string a = ast->sons[0]->val;
		string b = find_rez(ast->sons[1], mymap);
		mymap[a] = atof(b.c_str());
	}
	else if (ast->val == "true") {
		for (int i = 0; i < ast->sons.size(); i++)
			find_rez(ast->sons[i], mymap);
	}
	else if (ast->val == "false") {
		for (int i = 0; i < ast->sons.size(); i++)
			find_rez(ast->sons[i], mymap);
	}
	else if (ast->val == "if") {
		double cond;
		if (is_number(ast->sons[0]->val))
			cond = atof(ast->sons[0]->val.c_str());
		else
			cond = mymap.at(ast->sons[0]->val);
		if (cond)
			find_rez(ast->sons[1], mymap);
		else if (ast->sons.size() == 3)
			find_rez(ast->sons[2], mymap);
	}
	else if (is_operator(ast->val))
	{
		return optimization(ast, mymap);
	}
	else if (is_number(ast->val)) {
		return ast->val;
	}
	else
	{
		double rez = mymap.at(ast->val);
		std::ostringstream ost;
		ost << rez;
		return ost.str();
	}
	return "";
}
string optimization(Node* ast, map <string, double>& mymap) {

	if (ast->val == "*" && ast->sons[0]->sons.size() == 0 && is_zero(find_rez(ast->sons[0], mymap)))
		return "0";
	if (ast->val == "*" && ast->sons[1]->sons.size() == 0 && is_zero(find_rez(ast->sons[1], mymap)))
		return "0";
	if (ast->val == "/" && ast->sons[0]->sons.size() == 0 && is_zero(find_rez(ast->sons[0], mymap)))
		return "0";

	string a, b;
	if (ast->sons[0])
		a = find_rez(ast->sons[0], mymap);
	else
		a = "0";
	b = find_rez(ast->sons[1], mymap);
	double rez = calculator(ast->val, atof(a.c_str()), atof(b.c_str()));
	std::ostringstream ost;
	ost << rez;
	return ost.str();
}
bool is_zero(string s) {
	double x = atof(s.c_str());
	if (x < 0.0000001)
		return true;
	return false;
}

bool is_operator(string s) {
	if (s == "+" || s == "-" || s == "*" || s == "/" || s == "^")
		return true;
	return false;
}
void print_postf_bin(Node* root)
{
	if(root->sons.size() != 0)
		for (int i = 0; i < root->sons.size(); i++)
			print_postf_bin(root->sons[i]);
	cout << root->val;
}
void print_inf_bin(Node* root)
{
	if(root->sons.size() != 0)
		print_inf_bin(root->sons[0]);
	cout << root->val;
	if (root->sons.size() != 0)
		print_inf_bin(root->sons[1]);
}
void push_str(Stack** s, string data)
{
	Node* t = new Node;
	t->val = data;
	Stack* tmp = new Stack;
	tmp->data = t;
	tmp->next = *s;
	*s = tmp;
}
void push_node(Stack** s, Node** node)
{
	Stack* tmp = new Stack;
	tmp->data = *node;
	tmp->next = *s;
	*s = tmp;
}
string getFront(Stack* s)
{
	if (s == nullptr) return '\0';
	return s->data->val;
}

Node* pop(Stack** s)
{
	if (*s == nullptr) return nullptr;
	Node* t = (*s)->data;
	Stack* tmp = (*s)->next;
	delete (*s);
	(*s) = tmp;
	return t;
}

void apply_bin(Stack** operators, Stack** output)
{
	Node* result;
	string tmp;
	result = pop(operators);
	result->sons.resize(2);
	result->sons[1] = pop(output);
	result->sons[0] = pop(output);
	push_node(output, &result);
}

string getOp(char s)
{
	switch (s)
	{
	case '+':
		return "+";
	case '-':
		return "-";
	case '/':
		return "/";
	case '*':
		return "*";
	case '^':
		return "^";
	default:
		return "";
	}
}

string delete_space(string s) {
	string rez = "";
	for (int i = 0; i < s.length(); i++)
		if (s[i] != ' ')
			rez += s[i];
	return rez;
}

int getPriority(string op)
{
	if (op == "=")
		return 5;
	if (op == "(")
		return 4;
	if (op == "+" || op == "-")
		return 3;
	if (op == "/" || op == "*")
		return 2;
	if (op == "^")
		return 1;
}
Node* calc(string expr)
{
	Stack* operators = nullptr;
	Stack* output = nullptr;
	string result;
	expr = delete_space(expr);
	int i = 0;
	while (expr[i] != '\0')
	{
		if (expr[i] >= '0' && expr[i] <= '9' || expr[i] >= 'a' && expr[i] <= 'z')
		{
			string num = "";
			while (expr[i] >= '0' && expr[i] <= '9' || expr[i] >= 'a' && expr[i] <= 'z' || expr[i] == '.')
			{
				num += expr[i];
				i++;
			}
			//if(output != nullptr && getFront(output) == "-")*********************************
			//{
			//    push_str(&output, (pop(&output))->val + num);         пофиксить унарные "-"
			//}
			//*********************************************************************************
			//else
			push_str(&output, num);
			i--;
		}
		else if (getOp(expr[i]) != "")
		{
			string op = getOp(expr[i]);
			if ((i - 1 < 0 || expr[i - 1] < '0') && op == "-")
			{
				if (output == nullptr || getFront(output) != "-")
					push_str(&output, op);
				else pop(&output);
			}
			else
			{
				int p = getPriority(op);
				if (p != 1)
					while (operators != nullptr && p >= getPriority(getFront(operators)))
					{
						apply_bin(&operators, &output);
					}
				push_str(&operators, op);
			}
		}
		else if (expr[i] == '(')
		{
			push_str(&operators, "(");
		}
		else if (expr[i] == ')')
		{
			while (getFront(operators) != "(")
			{
				apply_bin(&operators, &output);
			}
			pop(&operators);
		}
		else if (expr[i] == '=')
		{
			push_str(&operators, "=");
		}
		/*else if (expr[i] == '!')
		{
		factorial(&output);
		}*/
		i++;
	}
	while (operators != nullptr)
	{
		apply_bin(&operators, &output);
	}
	return pop(&output);
}
