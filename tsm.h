#ifndef TSM_H
#define TSM_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

struct Edge {
    int u, v, w;
};

int getIndex(char c, const std::vector<char>& vertexList);
void TSP_Backtrack(int current, int start, int n, int depth, int curCost);
string TravelingBacktrack(int edgeList[][3], int numEdges, char start_char, const std::vector<char>& vertexList);
int TSP(const std::vector<Edge>& edges, const std::vector<char>& vertexList, int mask, int current, int start);
string TravelingDP(int edgeList[][3], int numEdges, char start_char, const std::vector<char>& vertexList, const std::vector<Edge>& edges);
vector<int> createInitialTour(int n, int start);
vector<int> twoOpt(std::vector<int> tour);
string TravelingGreedy(int edgeList[][3], int numEdges, char start_char, const std::vector<char>& vertexList, const std::vector<Edge>& edges);
string Traveling(int edgeList[][3], int numEdges, char start_char);

#endif 