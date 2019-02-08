#include <bits/stdc++.h>
#include "matriz.h"

using namespace std;

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

int main(int argc, char * argv[]) {
	// Decidir a forma de input das variaveis depois

	// O nome do arquivo de entrada esta no primeiro argumento

	if(argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 0;
	}

	// Abre o arquivo
	ifstream file(argv[1]);
	if(file.is_open() == 0) {
		printf("Could not open file %s\n", argv[1]);
		return 0;
	}

	string line;
	getline(file, line);

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

	// Removes all spaces from line, so there is no problem with that
	line.erase(remove(line.begin(), line.end(), ' '), line.end());

	vector<double> ct;
	vector<string> variables;
	int neg = 0;
	if(line[0] == '-') neg = 1; 
	for(int i = neg; i < line.size();) {
		double num = neg? -1 : 1;

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


			if(i == line.size() or line[i] != '*') throw runtime_error("Missing * in function description");
			i++;

			if(neg) num = -num;
		}
		ct.push_back(num);

		if(i == line.size()) throw runtime_error("Missing variable after coefficient");
		string var = "";
		var = line[i];
		if(isalpha(line[i]) == 0) throw runtime_error("Variables must start with alphabetic character");
		++i;
		while(i < line.size() and line[i] != '+' and line[i] != '-') {
			if(isalnum(line[i]) == 0) throw runtime_error("Variables must hae only alphanumeric characters");
			var += line[i++];
		}

		variables.push_back(var);

		if(i < line.size()) {
			if(line[i] != '+' and line[i] != '-') throw runtime_error("Unexpected character" + line[i]);
			neg = line[i] == '-';
			++i;
		}
	}

	for(double x : ct) cout << fixed << setprecision(6) <<  x << " ";
	cout << endl;

	for(string var : variables) cout << var << " ";
	cout << endl;

	file.close();

}