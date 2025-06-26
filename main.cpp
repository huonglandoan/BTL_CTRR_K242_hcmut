#include "bellman.h"
#include "tsm.h"
#include <random>
#define MAX 1000
using namespace std;

bool isExist(char c, char vertexList[], int count) {
    for (int i = 0; i < count; ++i) {
        if (vertexList[i] == c) return true;
    }
    return false;
}

void readEdgeListFromFile(const string& filename, int edgeList[][3], int& numEdges) {
    ifstream fin(filename);
    numEdges = 0;
    while (fin >> edgeList[numEdges][0] >> edgeList[numEdges][1] >> edgeList[numEdges][2]) {
        numEdges++;
    }
    fin.close();
}

void readEdgeListFromFile(const string& filename, int edgeList[][3], int& numEdges, char vertexList[], int& vertexCount) {
    ifstream fin(filename);
    numEdges = 0;
    vertexCount = 0;
    int u, v, w;

    while (fin >> u >> v >> w) {
        edgeList[numEdges][0] = u;
        edgeList[numEdges][1] = v;
        edgeList[numEdges][2] = w;

        char cu = char(u);
        char cv = char(v);

        if (!isExist(cu, vertexList, vertexCount)) {
            vertexList[vertexCount++] = cu;
        }
        if (!isExist(cv, vertexList, vertexCount)) {
            vertexList[vertexCount++] = cv;
        }

        numEdges++;
    }

    fin.close();
}

void printedge(int edge[]) {
    cout << char(edge[0]) << char(edge[1]) << "," << edge[2] << endl;
}

int main() {
    string files1[] = {"Input10A.txt", "Input20A.txt", "Input20B.txt"};

    for (int f = 0; f < 3; ++f) {
        int edgeList[MAX][3];
        int numEdges = 0;
        char vertexList[100];
        int vertexCount = 0;

        readEdgeListFromFile(files1[f], edgeList, numEdges, vertexList, vertexCount);

        cout << "===== Testing file: " << files1[f] << " =====\n";

        // Duyệt tất cả cặp đỉnh khác nhau
        for (int i = 0; i < vertexCount; ++i) {
            for (int j = 0; j < vertexCount; ++j) {
                if (i == j) continue;
                char start = vertexList[i];
                char end = vertexList[j];
                string path = BF_Path(edgeList, numEdges, start, end);
                cout << "Path from " << start << " to " << end << ": " << path << endl;
            }
        }

        cout << "========================================\n\n";
    }
    
    string files[] = {
        "Traveling15_1.txt",
        "Traveling15_2.txt",
        "Traveling20_1.txt",
        "Traveling20_2.txt",
        "Traveling25.txt"
    };

    for (int i = 0; i < 5; ++i) {
        int edgeList[MAX][3];
        int numEdges = 0;
        readEdgeListFromFile(files[i], edgeList, numEdges);

        char start_vertex = edgeList[0][0];  // Lấy đỉnh đầu tiên làm điểm bắt đầu
        cout << "===== " << files[i] << " =====\n";
        cout << Traveling(edgeList, numEdges, start_vertex) << endl << endl;
    }

    return 0;
}
