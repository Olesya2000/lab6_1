#include "Shunting-yard.h"

using namespace std;

string optimization(Node*, map <string, double>&);

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
				//////////////
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
	double x = atof(s.c_str());
	if (x < 0.0000001)
		return true;
	return false;
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

int main(int argc, char* argv[])
{
	ifstream f("file.txt");
	vector<Node*> ast;
	map <string, double> mymap;
	
	read(f, ast);
	for (int i = 0; i < ast.size(); i++)
		find_rez(ast[i], mymap);

/*
	for (int i = 0; i < ast.size(); i++)
	{
		if (ast[i]->val == "<<")
		{
			cout << "<<";
			print_inf_bin(ast[i]->sons[0]);
		}
		else
			print_inf_bin(ast[i]);
		cout << ';' << endl;
	}
*/
	system("pause");
	return 0;
}
