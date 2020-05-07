#include <iostream>
#include <fstream>
#include <sstream>

#include "digraph.h"

DigraphMatrix::DigraphMatrix(const string& filename)
{
    adjMatrix = NULL;

    bool bFirst = true;
    const int MAX_CHARS = 80;
    char line[MAX_CHARS + 1];

    ifstream ifs(filename.c_str());
    while(ifs.getline(line,MAX_CHARS))
    {
        istringstream iss(line);

        if(bFirst)
        {
            bFirst = false;

            iss >> numVertices;

            iss >> numEdges;

            adjMatrix = new bool*[numVertices];

            for (int vertex = 0; vertex < numVertices; vertex++)
            {
                adjMatrix[vertex] = new bool[numVertices];
                for (int v2 = 0; v2 < numVertices; v2++)
                    adjMatrix[vertex][v2] = false;
            }
        }
        else
        {
            int v1, v2;
            iss >> v1;
            iss >> v2;
            adjMatrix[v1][v2] = true;
        }
    }
}

DigraphMatrix::~DigraphMatrix()
{
    if(adjMatrix != NULL)
    {
        for(int vertex = 0; vertex < numVertices; vertex++)//free allocated memory
            delete[] adjMatrix[vertex];

        delete[] adjMatrix;
        adjMatrix = NULL;
    }
}

int DigraphMatrix::findEntryVertex(int prevEntry) const
{
    int vertex = -1;

    for(int v1 = prevEntry+1; vertex < 0 && v1 < numVertices; v1++)
    {
        int numE = 0;

        for(int v2 = 0; numE == 0 && v2 < numVertices; v2++)
        {
            if (adjMatrix[v2][v1])
            {
                numE++;
            }
        }

        if(numE == 0)
        {
            vertex = v1;
        }
    }

    return vertex;
}

int DigraphMatrix::findNeighbor(int v0, int prevN) const
{
    int v2 = -1;

    for(int v1 = prevN + 1; v2 < 0 && v1 < numVertices; v1++)//traverse vertices from prevN+1
    {
        if(adjMatrix[v0][v1])
        {
            v2 = v1;
        }
    }

    return v2;
}

void DigraphMatrix::DFS(int v0, bool* discovered, bool* explored, vector<int>& sorted) const
{
    discovered[v0] = true;

    int v1 = 0,prevN = -1;

    while(v1 >= 0)
    {
        v1 = findNeighbor(v0, prevN);
        prevN = v1;

        if(v1 >= 0) // found
        {
            if(discovered[v1] && !explored[v1])//a cycle
            {
                throw invalid_argument("Cycle at the graph");
            }
            else
            {
                DFS(v1, discovered, explored, sorted);//recursive call
            }    
        }
        else
        {
            if(!explored[v0])
            {
                explored[v0] = true;//if not added to sorted yet, add

                sorted.insert(sorted.begin(), v0);//in this case no need to reverse
            }
        }
    }
}

vector<int> DigraphMatrix::topologicalSort() const
{
    vector<int> sorted;//vector to return

    int v0 = -1, v1;

    bool* discovered = new bool[numVertices];
    bool* explored   = new bool[numVertices];
    
    for(v1 = 0; v1 < numVertices; v1++)
    {
        discovered[v1] = false;
        explored[v1] = false;
    }

    do // loop over the no-entry vertices
    {
        v0 = findEntryVertex(v0);

        if(v0 >= 0)
        {
            for(v1 = 0; v1 < numVertices; v1++)
                discovered[v1] = false;

            //a starting vertex found, perform DFS
            DFS(v0, discovered, explored, sorted);
        }
        else if((int)sorted.size() < numVertices)
        {
            throw invalid_argument("Cycle at the graph");
        }
    }
    while(v0 >= 0);

    //free memory
    delete[] discovered;
    delete[] explored  ;

    return sorted;
}
