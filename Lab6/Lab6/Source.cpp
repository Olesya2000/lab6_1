#include "AST.h"

using namespace std;


int main(int argc, char* argv[])
{
	ifstream f("file.txt");
	char ch;
	string s;
	vector<Node*> ast;
	bool assign = false;
	bool out = false;
	bool cond_start = false;
	while (f.get(ch))
	{
		if (ch != ';')
		{
			if (ch == '}')
			{
				cond_start = false;
				continue;
			}
			s += ch;
			if (ch == '=') assign = true;
			else if (s == "<<")
			{
				out = true;
				s.clear();
			}
			else if (s == "if")
			{
				string st;
				string t;
				cond_start = true;
				getline(f, st);
				s = delete_space(s);
				for (int i = 0; i < st.size(); i++)
				{
					if (st[i] == '(')
					{
						for (int j = i + 1; j < st.size(); j++)
						{
							if (st[j] == ')')
								break;
							t += st[j];
						}
						st.clear();
						break;
					}
				}
				Node* tmp;
				tmp = new Node;
				tmp->val = "if";
				Node* t1;
				t1 = new Node;
				t1->val = t;
				Node* t2;
				t2 = new Node;
				t2->val = "true";
				tmp->sons.push_back(t1);
				tmp->sons.push_back(t2);
				ast.push_back(tmp);
			}
			else if (s == "else")
			{
				Node* tmp;
				tmp = new Node;
				tmp->val = "false";
				ast.back()->sons.push_back(tmp);
				cond_start = true;
				s.clear();
			}
			else if (ch == '\n' || ch == '{' || s == " ") s.clear();
		}
		else if (cond_start)
		{
			if (assign)
			{
				ast.back()->sons.back()->sons.push_back(calc(s));
				assign = false;
				s.clear();
			}
			else if (out)
			{
				Node* o;
				o = new Node;
				o->val = "<<";
				o->sons.push_back(calc(s));
				ast.back()->sons.back()->sons.push_back(o);
				out = false;
				s.clear();
			}

		}
		else if (assign)
		{
			ast.push_back(calc(s));
			s.clear();
			assign = false;
		}
		else if (out)
		{
			Node* o;
			o = new Node;
			o->val = "<<";
			o->sons.push_back(calc(s));
			ast.push_back(o);
			out = false;
		}
	}
	for (int i = 0; i < ast.size(); i++)
	{
		if (ast[i]->val == "<<")
		{
			cout << "<<";
			print_inf_bin(ast[i]->sons[0]);
		}
		else if (ast[i]->val == "if")
		{
			cout << "if (" << ast[i]->sons[0]->val << ")" << endl;
			for (int j = 1; j < ast[i]->sons.size(); j++)
			{
				cout << ast[i]->sons[j]->val;
				cout << endl << "{" << endl;
				for (int k = 0; k < ast[i]->sons[j]->sons.size(); k++)
				{
					if (ast[i]->sons[j]->sons[k]->val == "<<")
					{
						cout << "<<";
						print_inf_bin(ast[i]->sons[j]->sons[k]->sons[0]);
					}
					else
						print_inf_bin(ast[i]->sons[j]->sons[k]);
					cout << ";" << endl;
				}
				cout << "}" << endl;
			}
		}

		else
			print_inf_bin(ast[i]);
		cout << ';' << endl;
	}
	system("pause");
	return 0;
}