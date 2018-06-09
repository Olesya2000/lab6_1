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