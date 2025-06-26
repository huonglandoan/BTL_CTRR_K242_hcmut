#include "tsm.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

#define INF 1000000000

int weight[25][25];
int minCost;
std::vector<int> curPath, bestPath;
bool visited[25];
int dp[1 << 20][20];
int next_city[1 << 20][20];

int getIndex(char c, const std::vector<char>& vertexList) {
    for (int i = 0; i < vertexList.size(); ++i)
        if (vertexList[i] == c)
            return i;
    return -1;
}

void TSP_Backtrack(int current, int start, int n, int depth, int curCost) {
    if (curCost >= minCost) return;

    if (depth == n) {
        if (weight[current][start] < INF) {
            int totalCost = curCost + weight[current][start];
            if (totalCost < minCost) {
                minCost = totalCost;
                bestPath = curPath;
                bestPath.push_back(start); 
            }
        }
        return;
    }

    for (int nxt = 0; nxt < n; ++nxt) {
        if (!visited[nxt] && weight[current][nxt] < INF) {
            visited[nxt] = true;
            curPath.push_back(nxt);
            TSP_Backtrack(nxt, start, n, depth + 1, curCost + weight[current][nxt]);
            curPath.pop_back();
            visited[nxt] = false;
        }
    }
}

std::string TravelingBacktrack(int edgeList[][3], int numEdges, char start_char, const std::vector<char>& vertexList) {
    int n = vertexList.size();
    int start = getIndex(start_char, vertexList);

    minCost = INF;
    bestPath.clear();
    curPath.clear();

    for (int i = 0; i < n; ++i)
        visited[i] = false;

    visited[start] = true;
    curPath.push_back(start);
    TSP_Backtrack(start, start, n, 1, 0);

    std::string result;
    if (bestPath.empty())
        result = "Error: No valid cycle found";
    else {
        for (int i = 0; i < bestPath.size(); ++i) {
            if (i) result += " ";
            result += vertexList[bestPath[i]];
        }
    }

    return result;
}
int TSP(const std::vector<Edge>& edges, const std::vector<char>& vertexList, int mask, int current, int start) {
    int n = vertexList.size();
    if (mask + 1 == (1 << n))
        return weight[current][start] < INF ? weight[current][start] : INF;

    if (dp[mask][current] != -1)
        return dp[mask][current];

    int bestCost = INF;
    int chosenNext = -1;

    for (int nxt = 0; nxt < n; ++nxt) {
        if ((mask & (1 << nxt)) == 0 && weight[current][nxt] < INF) {
            int cost = weight[current][nxt] + TSP(edges, vertexList, mask | (1 << nxt), nxt, start);
            if (cost < bestCost) {
                bestCost = cost;
                chosenNext = nxt;
            }
        }
    }

    dp[mask][current] = bestCost;
    next_city[mask][current] = chosenNext;
    return bestCost;
}
std::string TravelingDP(int edgeList[][3], int numEdges, char start_char, const std::vector<char>& vertexList, const std::vector<Edge>& edges) {
    int n = vertexList.size();
    int start = getIndex(start_char, vertexList);

    for (int mask = 0; mask < (1 << n); ++mask)
        for (int i = 0; i < n; ++i)
            dp[mask][i] = next_city[mask][i] = -1;

    int optimalCost = TSP(edges, vertexList, 1 << start, start, start);

    std::string resultPath(1, vertexList[start]);
    int visited = 1 << start;
    int current = start;

    while (true) {
        int nxt = next_city[visited][current];
        if (nxt == -1)
            break;
        resultPath += " ";
        resultPath += vertexList[nxt];
        visited |= (1 << nxt);
        current = nxt;
    }

    if (weight[current][start] < INF)
        resultPath += " " + std::string(1, vertexList[start]);

    return resultPath;
}
std::vector<int> createInitialTour(int n, int start) {
    std::vector<int> tour;
    tour.push_back(start);
    for (int i = 0; i < n; ++i)
        if (i != start)
            tour.push_back(i);
    tour.emplace_back(start);
    return tour;
}

std::vector<int> twoOpt(std::vector<int> tour) {
    bool improved = true;
    int size = tour.size();

    while (improved) {
        improved = false;
        for (int i = 1; i + 2 < size; ++i) {
            for (int j = i + 1; j + 1 < size; ++j) {
                int a = tour[i - 1], b = tour[i];
                int c = tour[j], d = tour[j + 1];

                if (weight[a][c] >= INF || weight[b][d] >= INF)
                    continue;

                bool valid = true;
                for (int x = i, y = j; x < y; ++x, --y) {
                    if (weight[tour[y]][tour[x]] >= INF) {
                        valid = false;
                        break;
                    }
                }

                if (!valid) continue;

                int costBefore = weight[a][b] + weight[c][d];
                int costAfter = weight[a][c] + weight[b][d];

                if (costAfter < costBefore) {
                    std::reverse(tour.begin() + i, tour.begin() + j + 1);
                    improved = true;
                }
            }
        }
    }

    return tour;
}

std::string TravelingGreedy(int edgeList[][3], int numEdges, char start_char, const std::vector<char>& vertexList, const std::vector<Edge>& edges) {
    int n = vertexList.size();
    int start = getIndex(start_char, vertexList);

    std::vector<int> tour = createInitialTour(n, start);
    tour = twoOpt(tour);

    bool validCycle = true;
    for (int i = 0; i + 1 < tour.size(); ++i) {
        if (weight[tour[i]][tour[i + 1]] >= INF) {
            validCycle = false;
            break;
        }
    }

    std::string result;
    if (validCycle) {
        for (int i = 0; i < tour.size(); ++i) {
            if (i) result += " ";
            result += vertexList[tour[i]];
        }
    } else {
        result = "Error: No valid cycle found";
    }

    return result;
}

std::string Traveling(int edgeList[][3], int numEdges, char start_char) {
    std::vector<char> vertexList;
    std::vector<Edge> edges;

    for (int i = 0; i < 25; ++i)
        for (int j = 0; j < 25; ++j)
            weight[i][j] = INF;

    for (int i = 0; i < numEdges; ++i) {
        char from = (char)edgeList[i][0];
        char to = (char)edgeList[i][1];
        int cost = edgeList[i][2];

        if (std::find(vertexList.begin(), vertexList.end(), from) == vertexList.end())
            vertexList.push_back(from);
        if (std::find(vertexList.begin(), vertexList.end(), to) == vertexList.end())
            vertexList.push_back(to);
    }

    std::sort(vertexList.begin(), vertexList.end());

    for (int i = 0; i < numEdges; ++i) {
        int u = getIndex((char)edgeList[i][0], vertexList);
        int v = getIndex((char)edgeList[i][1], vertexList);
        int w = edgeList[i][2];
        weight[u][v] = w;
        edges.push_back({u, v, w});
    }

    int nodeCount = vertexList.size();
    if (nodeCount <= 15)
        return TravelingBacktrack(edgeList, numEdges, start_char, vertexList);
    else if(nodeCount <= 20)
        return TravelingDP(edgeList, numEdges, start_char, vertexList, edges);
    else
        return TravelingGreedy(edgeList, numEdges, start_char, vertexList, edges);
}