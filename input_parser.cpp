#include "input_parser.h"

/* 
======== DESCRICAO DE INPUT ==========

O input da descricao do problema simplex sera sempre da mesma maneira:

1a linha: funcao para maximizar na forma: [numero *] <variavel 1> {+|-} [numero *] <variavel 2> {+|-} ... [numero *] <variavel m>
2a linha: em branco
proximas n linhas na forma:  [numero *] <variavel 1> {+|-} [numero *] <variavel 2> {+|-} ... [numero *] <variavel m> <= <numero>

Exemplo

file.in
---------------------
2 * a + 3 * c - x

a + c <= 15
3 * a - y <= 100
---------------------

Perceba que pode haver variaveis que aparecem na funcao e nao aparecem nas restricoes, e vice versa. Isso equivale a:

f(x) = 2 * a + 3 * c + 0 * y - 1 * x

Sujeito a:
1 * a + 1 * c + 0 * y + 0 * x <= 15
3 * a + 0 * c - 1 * y + 0 * x <= 100

Depois essas informacoes seram passadas para o README.md
*/

// Reads num
double read_num(string line, int & i) {
	double num = 1;
	// Le o coeficiente e coloca em numero
	if(isdigit(line[i])) { 
		// reads real number
		num = line[i++] - '0';
		while(i < line.size() and isdigit(line[i])) num = 10 * num + (line[i++] - '0');
		if(i < line.size() and line[i] == '.') {
			i++;
			if(i == line.size() or isdigit(line[i]) == 0) throw runtime_error("Missing decimal part of real number"); 
			double aux = 10.0;
			while(i < line.size() and isdigit(line[i])) {
				num += (line[i++] - '0')/aux;
				aux *= 10.0;
			}
		}
	}
	return num;
}

// Reads variable
string read_variable(string line, int & i) {
	if(i == line.size()) throw runtime_error("Missing variable after coefficient");
	string ans = "";
	ans += line[i];
	if(isalpha(line[i]) == 0) throw runtime_error("Variables must start with alphabetic character");
	++i;
	while(i < line.size() and line[i] != '+' and line[i] != '-' and line[i] != '<') {
		if(isalnum(line[i]) == 0) throw runtime_error("Variables must have only alphanumeric characters");
		ans += line[i++];
	}

	return ans;
}

void read_term(string line, int & i, double & x, string & var) {	
	int flag = isdigit(line[i]);
	x = read_num(line, i);
	if(flag) {
		if(i == line.size() or line[i] != '*') throw runtime_error("Missing * in function description");
		i++;
	}
	var = read_variable(line, i);
}

void parse_input(char * filename, map<string, double> & par_function, vector<map<string, double> > & par_inequations, vector<double> & par_b) {
	// Extracts function from line:
	/*
		Loop is

		1 -	a = next non-space character
		2 -	if(a == number) then
				2.1 - number = digits until '*'
				2.2 - Read '*' character
				2.3 - variable[0] = next character from alphabet
				2.4 - variable[1..n] = next alphanumeric characters until space
				2.5 - if(EOF)
					2.5.1 END
					  else
					2.5.2 Read '+' or '-' and go to step 1
		3 -	else then
				3.3 - variable[0] = next character from alphabet
				3.4 - variable[1..n] = next alphanumeric characters until EOF or '+' and '-'
				3.5 - if(EOF)
					3.5.1 END
					  else
					3.5.2 Read '+' or '-' and go to step 1
	*/

	// Abre o arquivo
	ifstream file(filename);
	if(file.is_open() == 0) {
		printf("Could not open file %s\n", filename);
		exit(0);
	}

	string line;
	getline(file, line);

	// Removes all spaces from line, so there is no problem with that
	line.erase(remove(line.begin(), line.end(), ' '), line.end());

	map<string, double> function; // function[x] = coefficient of variable called x in function description
	int neg = 0;
	if(line[0] == '-') neg = 1; 
	for(int i = neg; i < line.size();) {
		double num;
		string var = "";
		read_term(line, i, num, var);

		if(neg) num = -num;
		if(function.count(var)) throw runtime_error("Variable " + var + " appears more than once");
		function[var] = num;

		if(i < line.size()) {
			if(line[i] != '+' and line[i] != '-') throw runtime_error("Unexpected character " + line[i]);
			neg = line[i] == '-';
			++i;
		}
	}

	getline(file, line);
	if(line.size()) throw runtime_error("Missing blank line after function description");

	// ======================== Leitura das inequacoes ========================

	vector<map<string, double > > inequations;
	vector<double> b;
	while(file.eof() == 0) {
		getline(file, line);
		if(line.size() == 0) break;
		inequations.emplace_back();
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		neg = 0;
		if(line[0] == '-') neg = 1;
		int i;
		for(i = neg; i < line.size();) {
			double num;
			string var;

			read_term(line, i, num, var);
			if(neg) num = -num;

			if(inequations.back().count(var)) throw runtime_error("Variable " + var + " appears more than once");
			inequations.back()[var] = num;
			if(line[i] == '<') break;

			if(i < line.size()) {
				if(line[i] != '+' and line[i] != '-') throw runtime_error("Unexpected character " + line[i]);
				neg = line[i] == '-';
				++i;
			}
		}

		i++;
		if(i >= line.size()) throw runtime_error("Missing inequation right-side");
		if(line[i] != '=') throw runtime_error("Unexpected character " + line[i]);
		i++;
		if(i >= line.size()) throw runtime_error("Missing inequation right-side");

		neg = 0;
		if(line[i] == '-' or line[i] == '+') neg = line[i++] == '-';
		double num = read_num(line, i);
		if(neg) num = -num;
		b.push_back(num);
		if(i != line.size()) throw runtime_error("Unexpected character " + line[i]);
	} 

	file.close();

	par_function = function;
	par_inequations = inequations;
	par_b = b;
}