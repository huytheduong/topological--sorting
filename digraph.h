#ifndef __DIGRAPH_H
#define __DIGRAPH_H

#include <string>
#include <vector>
using namespace std;

class DigraphMatrix
{
public:
  DigraphMatrix(const string& filename);
  vector<int> topologicalSort() const;
 ~DigraphMatrix();

private:
    int numVertices, numEdges;
    bool** adjMatrix;
    int findEntryVertex(int prevEntry) const;
    int findNeighbor(int v0, int prevN) const;
    void DFS(int v0, bool* discovered, bool* explored, vector<int>& sorted) const;
};

#endif