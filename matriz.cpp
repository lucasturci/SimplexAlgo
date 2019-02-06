#include "matriz.h"
#include <vector>
#include <iostream>

using namespace std;

// Finds solution for system Ax = b (ans = x), if any. Returns 0 in case are no solutions, 1 otherwise
int gauss_elimination(vector<vector<double> > A, vector<double> b,  vector<double> & ans) {
	const double eps = 1e-9;
	int n = A.size(), m = A[0].size();
	for(int i = 0; i < n; ++i) // Concatenates b to A, for code improvement
		A[i].push_back(b[i]);

	vector<int> where (m, -1); // For each variable xi, the value where(i) is the line where this column is not zero. This vector is needed because some variables can be independent.
	for(int col = 0, row = 0; col < m and row < n; ++col) {
		int sel = row;
		for(int i = row; i < n; ++i) // take best pivot
			if(abs(A[i][col]) > abs(A[sel][col])) sel = i;

		if(abs(A[sel][col]) < eps) continue; // independent variable
		for(int i = col; i <= m; ++i)
			swap(A[sel][i], A[row][i]);
		where[col] = row;

		for(int i = 0; i < n; ++i)
			if(i != row) {
				double c = A[i][col] / A[row][col];
				for(int j = col; j <= m; ++j)
					A[i][j] -= A[row][j] * c;
			}
		row++;
	}

	ans.assign(m, 0);
	for(int i = 0; i < m; ++i)
		if(where[i] != -1) // variavel nao eh independente
			ans[i] = A[where[i]][m] / A[where[i]][i];

	// Now checks if found solution is really working
	for(int i = 0; i < n; ++i) {
		double sum = 0;
		for(int j = 0; j < m; ++j)
			sum += ans[j] * A[i][j];
		if(abs(sum - A[i][m]) > eps) return 0;
	}

	return 1;
}

vector<vector<double> > transpose(const vector<vector<double> > & A) {
	vector< vector<double> > ans;

	int n = A.size(), m = A[0].size();
	ans = vector<vector<double> > (m, vector<double> (n));

	for(int i = 0; i < n; i++)
		for(int j = 0; j < m; j++)
			ans[j][i] = A[i][j];
	return ans;
}