#include "Shunting-yard.h"
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
	if (!s) return "\0";
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
	result = pop(operators);
	Node* x = pop(output);
	result->sons.push_back(pop(output));
	result->sons.push_back(x);
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
	case '«':
		return "«";
	case '=':
		return "=";
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

bool is_operator(string s) {
	if (s == "+" || s == "-" || s == "*" || s == "/" || s == "^")
		return true;
	return false;
}
int getPriority(string op)
{
	if (op == "«")
		return 6;
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
	expr = delete_space(expr);
	int i = 0;
	while (expr[i] != '\0')
	{
		if ((expr[i] >= '0' && expr[i] <= '9' )||( expr[i] >= 'a' && expr[i] <= 'z'))
		{
			string num = "";
			while ((expr[i] >= '0' && expr[i] <= '9') || (expr[i] >= 'a' && expr[i] <= 'z') || expr[i] == '.')
			{
				num += expr[i];
				i++;
			}
			push_str(&output, num);
			i--;
		}
		else if (getOp(expr[i]) != "")
		{
			string op = getOp(expr[i]);
			/*if((i-1 < 0 || expr[i-1] < '0') && op == "-")
			{
                if(output == nullptr || getFront(output) != "-")
                push_str(&output, op);
                else pop(&output);
			}
			else*/
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
		i++;
	}
	while (operators != nullptr)
	{
		apply_bin(&operators, &output);
	}
	return pop(&output);
}
