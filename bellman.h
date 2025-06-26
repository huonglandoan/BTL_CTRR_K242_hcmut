#ifndef BELLMAN_H
#define BELLMAN_H

#include <iostream>
#include <string>
using namespace std;

void BF(int edgeList[][3], int numEdges, char start_vertex, int BFValue[], int BFPrev[]);
string BF_Path(int edgeList[][3], int numEdges, char startChar, char goalChar); 

#endif