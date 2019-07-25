#include <iostream>
#include <vector>
#include <string>

int main()
{
	std::string raw, out;
	std::cin >> raw;
	for (auto c : raw)
	{
		switch (c)
		{
		case '+':
			out += "Bruh";
			break;
		case '-':
			out += "BRuh";
			break;
		case '>':
			out += "bRuh";
			break;
		case '<':
			out += "bRUh";
			break;
		case '[':
			out += "brUh";
			break;
		case ']':
			out += "brUH";
			break;
		case '.':
			out += "bruH";
			break;
		case ',':
			out += "BruH";
			break;
		default:
			out += c;
			break;
		}
	}
	std::cout << out;
	return 0;
}