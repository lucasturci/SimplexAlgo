#ifndef MATRIZ_H
#define MATRIZ_H

#include <cstdlib>
#include <vector>
using namespace std;

int gauss_elimination(vector<vector<double> > A, vector<double> b, vector<double> & ans);
vector<vector<double> > transpose(const vector<vector<double> > & A);

#endif