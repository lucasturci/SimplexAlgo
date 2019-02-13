#include <bits/stdc++.h>
#include "matriz.h"
#include "input_parser.h"

#define EPS 1e-9
using namespace std;


double solve(vector<double> cT, vector<vector<double> > A, vector<double> b, vector<double> & xB) {
	double res = 0.0;

	int N = b.size();
	int M = cT.size() - N;

	set<int> basic, non_basic;

	// Initial feasible solution, all real variables are zeroed, therefore go to non_basic set
	for(int i = 0; i < M; ++i) non_basic.insert(i);
	for(int i = M; i < N + M; ++i) basic.insert(i);

	bool optimal = false;

	while(!optimal) {
		// Step 0: Calculate required variables before start

		// Calculate B (basic partition of A)
		vector<vector<double> > B(N, vector<double> (N));
		int aux = 0;
		for(int j : basic) {
			for(int i = 0; i < N; ++i)
				B[i][aux] = A[i][j]; 
			aux++;
		}

		// Calculate cB (basic partition of cT)
		vector<double> cB(N);
		aux = 0;
		for(int j : basic)
			cB[aux++] = cT[j];

		// Calculate xB (basic partition of answer)
		if(!gauss_elimination(B, b, xB)) throw runtime_error("No solutions for system B * xB = b");

		// Transposed B
		vector<vector<double> > BT = transpose(B);
		// Calculate Y =  cB * B^-1
		vector<double> Y;
		if(!gauss_elimination(BT, cB, Y)) throw runtime_error("No solutions for system BT * Y = cB");

		// ============================== Step 1 : Check optimality ================================


		int k;
		optimal = true;
		// Calculate cj : relative cost of variable j
		for(int j : non_basic) {
			double cj = cT[j];
			for(int i = 0; i < N; ++i) {
				cj -= Y[i] * A[i][j];
			}
			if(cj > EPS) {
				optimal = false;
				k = j;
			}
		}

		// ========================== Step 2 : Find next basic solution ============================

		if(!optimal) {
			// Variable k can be put into basic set

			vector<double> Nk(N); // kth column of N
			for(int i = 0; i < N; ++i) Nk[i] = A[i][k];

			// Calculate Z = B^-1 * Nk (kth column of N)
			vector<double> Z;
			if(!gauss_elimination(B, Nk, Z)) throw runtime_error("No solutions for system B * Z = Nk");

			// Calculate delta
			double delta = 1e30;
			aux = 0;
			for(int j : basic) {
				if(Z[aux] > EPS) delta = min(delta, xB[aux]/Z[aux]);
				aux++;
			}

			// Infinite solution
			if(delta == 1e30) {
				cout << "There is no real solution\n";
				exit(0);
			}

			// Remove first zeroed variable from basic set
			int out;
			aux = 0;
			for(int j : basic) {
				if(xB[aux] == delta * Z[aux]) {
					out = j;
					break;
				}
				aux++;
			}

			// Swap k and out between sets
			non_basic.erase(k);
			basic.insert(k);
			basic.erase(out);
			non_basic.insert(out);
		} else {
			res = 0.0;
			for(int i = 0; i < N; ++i) res += Y[i] * b[i];
		}
	}

	return res;
}

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