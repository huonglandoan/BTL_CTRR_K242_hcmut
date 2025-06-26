#include "bellman.h"
using namespace std;

void BF(int edgeList[][3], int numEdges, char start_vertex, int BFValue[], int BFPrev[]) {
    static int vertexList[100];
    static int vertexCount = 0;
    static bool initialized = false;
    static int G[100][100];
    static int v2i[256];   
    static int i2v[100];   
    static int usedSize = 0;

    static int prevNumEdges = -1;
    static char prevStartVertex = 0;

    if (numEdges != prevNumEdges || start_vertex != prevStartVertex) {
        vertexCount = 0;
        usedSize = 0;
        initialized = false;
        for (int i = 0; i < 256; ++i) v2i[i] = -1;
        for (int i = 0; i < 100; ++i)
            for (int j = 0; j < 100; ++j)
                G[i][j] = -1;

        prevNumEdges = numEdges;
        prevStartVertex = start_vertex;
    }

    auto getIndex = [](int ch, int list[], int count) {
        for (int i = 0; i < count; ++i)
            if (list[i] == ch) return i;
        return -1;
    };

    if (!initialized) {
        for (int i = 0; i < numEdges; ++i) {
            int u = edgeList[i][0], v = edgeList[i][1];
            if (getIndex(u, vertexList, vertexCount) == -1)
                vertexList[vertexCount++] = u;
            if (getIndex(v, vertexList, vertexCount) == -1)
                vertexList[vertexCount++] = v;
        }

        for (int i = 0; i < vertexCount - 1; ++i)
            for (int j = i + 1; j < vertexCount; ++j)
                if (vertexList[i] > vertexList[j]) {
                    int tmp = vertexList[i];
                    vertexList[i] = vertexList[j];
                    vertexList[j] = tmp;
                }

        // Ánh xạ tối đa Esize đỉnh đầu tiên
        for (int i = 0; i < vertexCount && usedSize < 100; ++i) {
            int ch = vertexList[i];
            v2i[ch] = usedSize;
            i2v[usedSize] = ch;
            usedSize++;
        }

        for (int i = 0; i < numEdges; ++i) {
            int u = edgeList[i][0], v = edgeList[i][1], w = edgeList[i][2];
            if (v2i[u] != -1 && v2i[v] != -1) {
                G[v2i[u]][v2i[v]] = w;
            }
        }

        initialized = true;
    }

    int startIdx = v2i[start_vertex];
if (startIdx == -1) {
    bool found = false;
    for (int i = 0; i < numEdges; ++i) {
        if (edgeList[i][0] == start_vertex || edgeList[i][1] == start_vertex) {
            found = true;
            break;
        }
    }
    if (!found) {
        BFPrev[usedSize - 1] = 0;
        return;
    }
}

    bool firstCall = true;
    for (int i = 0; i < usedSize; ++i)
        if (BFValue[i] != -1 || BFPrev[i] != -1) {
            firstCall = false;
            break;
        }

    if (firstCall) {
    for (int i = 0; i < usedSize; ++i) {
        BFValue[i] = -1;
        BFPrev[i] = -1;
    }
    BFValue[startIdx] = 0;
}

    int temp[100];
    for (int i = 0; i < usedSize; ++i)
        temp[i] = BFValue[i];

    for (int u = 0; u < usedSize; ++u) {
        if (BFValue[u] == -1) continue;
        for (int v = 0; v < usedSize; ++v) {
            if (G[u][v] != -1) {
                int newDist = BFValue[u] + G[u][v];
                if (temp[v] == -1 || newDist < temp[v]) {
                    temp[v] = newDist;
                    BFPrev[v] = u;
                    
                }
            }
        }
    }

    for (int i = 0; i < usedSize; ++i)
        BFValue[i] = temp[i];
}

string BF_Path(int edgeList[][3], int numEdges, char startChar, char goalChar) {
    const int MAXV = 100;
    int vertexList[MAXV], vertexCount = 0;
    int v2i[256];   
    int i2v[MAXV];  
    int usedSize = 0;

    for (int i = 0; i < 256; ++i) v2i[i] = -1;

    for (int i = 0; i < numEdges; ++i) {
        int u = edgeList[i][0], v = edgeList[i][1];
        if (v2i[u] == -1) {
            v2i[u] = vertexCount;
            i2v[vertexCount] = u;
            vertexList[vertexCount++] = u;
        }
        if (v2i[v] == -1) {
            v2i[v] = vertexCount;
            i2v[vertexCount] = v;
            vertexList[vertexCount++] = v;
        }
    }

    sort(vertexList, vertexList + vertexCount);

    for (int i = 0; i < vertexCount; ++i) {
        v2i[vertexList[i]] = i;
        i2v[i] = vertexList[i];
    }
    usedSize = vertexCount;

    int G[MAXV][MAXV];
    for (int i = 0; i < usedSize; ++i)
        for (int j = 0; j < usedSize; ++j)
            G[i][j] = -1;

    for (int i = 0; i < numEdges; ++i) {
        int u = edgeList[i][0], v = edgeList[i][1], w = edgeList[i][2];
        int uIdx = v2i[u], vIdx = v2i[v];
        G[uIdx][vIdx] = w;
    }

    // Bellman-Ford
    int BFValue[MAXV], BFPrev[MAXV];
    for (int i = 0; i < usedSize; ++i) {
        BFValue[i] = -1;
        BFPrev[i] = -1;
    }

    int startIdx = v2i[startChar];
    int goalIdx = v2i[goalChar];
    BFValue[startIdx] = 0;

    for (int step = 0; step < usedSize - 1; ++step) {
        int temp[MAXV];
        for (int i = 0; i < usedSize; ++i)
            temp[i] = BFValue[i];

        for (int u = 0; u < usedSize; ++u) {
            if (BFValue[u] == -1) continue;
            for (int v = 0; v < usedSize; ++v) {
                if (G[u][v] != -1) {
                    int newDist = BFValue[u] + G[u][v];
                    if (temp[v] == -1 || newDist < temp[v]) {
                        temp[v] = newDist;
                        BFPrev[v] = u;
                    }
                }
            }
        }

        for (int i = 0; i < usedSize; ++i)
            BFValue[i] = temp[i];
    }

    if (BFValue[goalIdx] == -1)
        return "No path";

    vector<int> path;
    int current = goalIdx;
    while (current != startIdx) {
        path.push_back(current);
        current = BFPrev[current];
        if (current == -1) return "No path";
    }
    path.push_back(startIdx);
    reverse(path.begin(), path.end());

    string result = "";
    for (size_t i = 0; i < path.size(); ++i) {
        result += (char)(i2v[path[i]]);
        if (i != path.size() - 1)
            result += " ";
    }
    return result;
}
