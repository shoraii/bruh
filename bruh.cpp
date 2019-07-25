#include <stdio.h>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctype.h>

//typedefs
typedef char WORD;
typedef std::vector<WORD> instruction_t;
typedef instruction_t::iterator instruction_pointer;

typedef std::vector<WORD> memory_t;
typedef memory_t::iterator memory_pointer;


//structs
struct environment
{
	memory_t memory;
	instruction_t instructions;

	instruction_pointer ip;
	memory_pointer mp;

	environment() : memory(3000)
	{
		clear();
		mp = memory.begin();
	}
	void clear()
	{
		instructions.clear();
		ip = instructions.begin();
	}
};

//prototypes
void interpret(environment& env);
int from_line(environment& env, std::string& line, int open_brackets = 0);
int from_stream(environment& env, std::istream& stream, int open_brackets = 0);
void interactive_mode(environment& env);
void print_word(WORD word);
std::string bruhtobf(std::string raw);

int main(int argc, char** argv)
{
	environment env;
	interactive_mode(env);
	return 0;
}

std::string bruhtobf(std::string raw)
{
	int loop_counter = 0;
	std::string word = "";
	std::string output = "";
	for (auto i : raw)
	{
		word += i;
		loop_counter++;
		if (loop_counter % 4 == 0)
		{
			loop_counter = 0;
			if (word == "Bruh")
			{
				output += '+';
			}
			else if (word == "BRuh")
			{
				output += '-';
			}
			else if (word == "bRuh")
			{
				output += '<';
			}
			else if (word == "bRUh")
			{
				output += '>';
			}
			else if (word == "brUh")
			{
				output += '[';
			}
			else if (word == "brUH")
			{
				output += ']';
			}
			else if (word == "bruH")
			{
				output += '.';
			}
			else if (word == "BruH")
			{
				output += ',';
			}
			else
			{
				throw std::exception();
			}
			word = "";
		}
	}
	return output;
}

void print_word(WORD word)
{
	if (isprint(word))
		std::cout << word;
	else
		std::cout << "0x" << std::hex << (int)word << std::dec;
}

int from_line(environment& env, std::string& line, int open_brackets)
{
	std::istringstream stream = std::istringstream(line);
	return from_stream(env, stream, open_brackets);
}

int from_stream(environment& env, std::istream& stream, int open_brackets)
{
	if (open_brackets == 0)
		env.instructions.push_back('\0');
	WORD word;
	while (true)
	{
		stream >> word;
		if (!stream) break;
		switch (word)
		{
		case '<': case '>': case '+': case '-': case ',': case '.':
			env.instructions.push_back(word);
			break;
		case '[':
			open_brackets++;
			env.instructions.push_back(word);
			break;
		case ']':
			open_brackets--;
			env.instructions.push_back(word);
			break;
		default:
			break;
		}
	}
	if (open_brackets == 0)
		env.instructions.push_back('\0');
	return open_brackets;
}

void interactive_mode(environment& env)
{
	int open_brackets = 0;
	while (true)
	{
		std::string line;
		std::getline(std::cin, line);
		line = bruhtobf(line);
		if (!std::cin) break;

		if (open_brackets == 0) env.clear();
		open_brackets = from_line(env, line, open_brackets);

		if (open_brackets == 0)
		{
			interpret(env);
			std::cout << ">";
			print_word(*env.mp);
			std::cout << std::endl;
		}
		else
		{
			std::cout << "->";
		}
	}
}

void find_closing(environment& env)
{
	int balance = 1;
	do
	{
		env.ip++;
		if (*env.ip == '[')
			balance++;
		else if (*env.ip == ']')
			balance--;

	} while (balance != 0);
}

void find_opening(environment& env)
{
	int balance = 0;
	do
	{
		if (*env.ip == '[')
			balance++;
		else if (*env.ip == ']')
			balance--;
		env.ip--;
	} while (balance != 0);
}


void interpret(environment& env)
{
	env.ip = env.instructions.begin();
	while (env.ip != env.instructions.end())
	{
		switch (*env.ip)
		{
		case '+':
			(*env.mp)++;
			env.ip++;
			break;
		case '-':
			(*env.mp)--;
			env.ip++;
			break;
		case '>':
			if (env.mp != (env.memory.end()--))
				env.mp++;
			env.ip++;
			break;
		case '<':
			if (env.mp != env.memory.begin())
				env.mp--;
			env.ip++;
			break;
		case '.':
			print_word(*env.mp);
			env.ip++;
			break;
		case ',':
			WORD word;
			std::cin >> word;
			(*env.mp) = word;
			env.ip++;
			break;
		case '[':
			if (!(*env.mp))
				find_closing(env);
			env.ip++;
			break;
		case ']':
			find_opening(env);
			env.ip++;
			break;
		case '\0':
			env.ip++;
			break;
		}
	}
}