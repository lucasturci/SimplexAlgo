#include <bits/stdc++.h>
#include "matriz.h"
#include "input_parser.h"

using namespace std;

int main(int argc, char * argv[]) {
	// O nome do arquivo de entrada esta no primeiro argumento
	if(argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return 0;
	}


	map<string, double> function;
	vector<map<string, double> > inequations;
	vector<double> b;

	parse_input(argv[1], function, inequations, b);

	/*
		Espaço pra pensar
		Agora eu tenho um map de string para double, que guarda a relacao no map[x] = y - coeficiente que acompanha a variavel x eh igual a y
		Isso eu tenho pra toda linha, e o vetor b
	*/

	// ========= Lets build matrix A and vector cT (c transposed) ==========

	// Lets put all of known variables inside vector variables
	set<string> set_of_variables;
	vector<string> variables;

	for(pair<string, double> aux : function)
		set_of_variables.insert(aux.first);

	for(int i = 0; i < inequations.size(); ++i)
		for(pair<string, double> aux : inequations[i])
			set_of_variables.insert(aux.first);

	variables.assign(set_of_variables.begin(), set_of_variables.end());

	// Dimensions of matrix A
	int M = variables.size();
	int N = inequations.size();

	// Builds matrix A
	vector<vector<double> > A = vector<vector<double> > (N, vector<double>(N + M));
	for(int i = 0; i < N; ++i) {
		for(int j = 0; j < M; ++j) {
			string & var = variables[j];
			A[i][j] = inequations[i][var];
		}
		for(int j = M; j < N + M; ++j)
			A[i][j] = j == i + M;
	}

	// Builds vector cT;
	vector<double> cT = vector<double> (N + M, 0);
	for(int i = 0; i < M; ++i)
		cT[i] = function[variables[i]];


	/* Parte de testes */

	cout << "Variaveis: ";
	for(string x : variables) cout << x << " ";
	cout << endl << endl;

	cout << "Vector cT: ";
	for(double x : cT) cout << x << " ";
	cout << endl << endl;
	
	cout << "Matriz A:\n";
	for(int i = 0; i < N; ++i) {
		for(int j = 0; j < N + M; ++j) {
			cout << A[i][j] << " ";
		}
		cout << endl;
	}

	exit(0);

	for(pair<string, double> aux : function) {
		printf("(%s, %.6lf) ", aux.first.c_str(), aux.second);
	}
	cout << endl;

	for(int i = 0; i < inequations.size(); ++i) {
		printf("Inequacao %d: ", i + 1);
		for(pair<string, double> aux : inequations[i]) {
			printf("(%s, %.6lf) ", aux.first.c_str(), aux.second);
		}
		cout << endl;
	}

	for(double x : b) cout << x << " ";
	cout << endl;
}