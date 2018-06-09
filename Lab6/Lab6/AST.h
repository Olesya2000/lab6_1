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
Node* calc(string);