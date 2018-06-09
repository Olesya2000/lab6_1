#include "Shunting-yard.h"

using namespace std;

void optimization(Node*);

bool in_if = false; //
void read(ifstream& f, vector<Node*>& ast) {
	char ch;
	string s;
	bool cond_start = false;
	while (!in_if && f.get(ch))
	{
		if (ch != ';')
		{
			///////////
			if (ch == '}') {
				string s1;
				getline(f, s1);
				if (s1 != "else")
					in_if = true;
				return;
			}
			if (ch == '{') {
				Node* temp = new Node;
				temp->val = "#";
				ast.push_back(temp);
				read(f, temp->sons);
				s.clear();
			}
			if (ch != ' ')
				s += ch;
			if (s == "if") {
				Node* temp;
				temp = new Node;
				temp->val = "?";
				string s1;
				getline(f, s1);
				temp->sons.push_back(calc(s1));
				ast.push_back(temp);
				read(f, temp->sons);
				s.clear();
			}
			if (s == "else") {
				s.clear();
			}
			else if (s == "<<")
				s = "«";
			else if (s == "\n") s.clear();
		}
		else {
			ast.push_back(calc(s));
			s.clear();
		}
	}
	if (in_if) {
		in_if = false;
		return;
	}
}

bool is_number(string s) {
	for (int i = 0; i < s.length(); i++)
		if (!((s[i] >= '0' && s[i] <= '9') || s[i] == '.'))
			return false;
	return true;
}

bool is_zero(string s) {
	if (s == "0" || s == "0.0" || s == "0.00")
		return true;
	return false;
}

void output(vector<Node*> ast)
{
	for (int i = 0; i < ast.size(); i++)
	{
		if (ast[i]->val == "«")
		{
			cout << "<<";
			print_inf_bin(ast[i]->sons[1]);
		}
		else if (ast[i]->val == "?")
		{
			cout << "if (" << ast[i]->sons[0]->val << ")" << endl;
			for (int j = 1; j < ast[i]->sons.size(); j++)
			{
				if (j == 2)
					cout << "else" << endl;
				cout << "{" << endl;
				output(ast[i]->sons[j]->sons);
				cout << "}" << endl;
			}
		}

		else
			print_inf_bin(ast[i]);
		cout << ';' << endl;
	}
}

string find_rez(Node* ast, map <string, double>& mymap) {
	if (ast->val == "«")
		cout << find_rez(ast->sons[1], mymap) << endl;
	else if (ast->val == "=") {
			string a = ast->sons[0]->val;
			string b = find_rez(ast->sons[1], mymap);
			mymap[a] = atof(b.c_str());
	}
	else if (ast->val == "#") {
		for (int i = 0; i < ast->sons.size(); i++)
			find_rez(ast->sons[i], mymap);
	}
	else if (ast->val == "?") {
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
	else if(is_operator(ast->val))
	{
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

void optimization(Node* ast) {

	if (ast->val == "*" && is_zero(ast->sons[0]->val)) {
		ast->val = "0";
		ast->sons.clear();
	}
	else
	if (ast->val == "*" && is_zero(ast->sons[1]->val)) {
		ast->val = "0";
		ast->sons.clear();
	}
	else
	if (ast->val == "/" && is_zero(ast->sons[0]->val))
	{
		ast->val = "0";
		ast->sons.clear();
	}
	else {
		for (int i = 0; i < ast->sons.size(); i++)
			if (ast->sons[i])
				optimization(ast->sons[i]);
	}
}

int main(int argc, char* argv[])
{
	ifstream f("file.txt");
	vector<Node*> ast;
	map <string, double> mymap;
	
	read(f, ast);

	output(ast);

	cout << endl << endl << endl;

	for (int i = 0; i < ast.size(); i++)
		if (ast[i])
			optimization(ast[i]);

	output(ast);
	for (int i = 0; i < ast.size(); i++)
		find_rez(ast[i], mymap);

	system("pause");
	return 0;
}
