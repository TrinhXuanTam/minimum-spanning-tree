#include <vector>
#include <fstream>
#include <iostream>
#include <climits>

using namespace std; 

struct Edge 
{
    Edge(const int& v1, const int& v2, const int& weight) 
    : v1(v1), v2(v2), weight(weight) {}

    int v1;
    int v2;
    int weight;
};

class WeightedGraph 
{
public:
    WeightedGraph(const vector<Edge>& edges, const vector<vector<int>>& adjacencyMatrix)
    : edges(edges), adjacencyMatrix(adjacencyMatrix) {}

    WeightedGraph(const vector<Edge>& edges, const int& vertexCnt) : edges(edges)
    {   
        this->adjacencyMatrix = vector<vector<int>>(vertexCnt, vector<int>(vertexCnt));
        for (const Edge& edge : this->edges)
        {
            this->adjacencyMatrix[edge.v1][edge.v2] = edge.weight;
            this->adjacencyMatrix[edge.v2][edge.v1] = edge.weight;
        }
    }

    static WeightedGraph fromFile(const string& path)
    {
        ifstream file(path);
        if (!file.is_open()) 
        {
            throw "Could'n open given file.";
        }
        
        int vertexCnt;
        file >> vertexCnt;
        vector<Edge> edges;
        vector<vector<int>> adjacencyMatrix(vertexCnt, vector<int>(vertexCnt));

        for (int i = 0; i < vertexCnt; ++i) 
        {
            for (int j = 0; j < vertexCnt; ++j) 
            {
                int weight;
                file >> weight;
                adjacencyMatrix[i][j] = weight;
                if (weight != 0 && j >= i) 
                {
                    edges.emplace_back(i, j, weight);
                }
            }
        }

        return WeightedGraph(edges, adjacencyMatrix);
    }

    int getWeight() const
    {
        int totalWeight = 0;
        for (const Edge& edge : this->edges)
        {
            totalWeight += edge.weight;
        }
        return totalWeight;
    }

    WeightedGraph getMinimumSpanningTree() const
    {
        int vertexCnt = this->adjacencyMatrix.size();
        vector<Edge> pickedEdges;
        vector<bool> pickedVertices(vertexCnt, false);
        vector<int> vertexPrize(vertexCnt, INT_MAX);
        vertexPrize[0] = 0;

        for (int i = 0; i < vertexCnt - 1; i++)
        {
            int cheapestVertex;
            int minPrice = INT_MAX;
            for (int j = 0; j < vertexCnt; j++)
            {
                if (!pickedVertices[j] && vertexPrize[j] < minPrice)
                {
                    minPrice = vertexPrize[j];
                    cheapestVertex = j;
                }
            }
            pickedVertices[cheapestVertex] = true;

            for (int j = 0; j < vertexCnt; j++)
            {
                if (this->adjacencyMatrix[cheapestVertex][j] != 0 && !pickedVertices[j] && this->adjacencyMatrix[cheapestVertex][j] < vertexPrize[j])
                {
                    vertexPrize[j] = this->adjacencyMatrix[cheapestVertex][j];
                    pickedEdges.emplace_back(cheapestVertex, j, this->adjacencyMatrix[cheapestVertex][j]);
                }
            }
        }

        return WeightedGraph(pickedEdges, vertexCnt);
    }
private:
    vector<Edge> edges;
    vector<vector<int>> adjacencyMatrix;
};

int main(int argc, char **argv)
{
    if (argc < 2) 
    {
        cout << "Please specify the input file path." << endl;
        return 1;
    }

    cout << "Input file: '" << argv[1] << "'" << endl;
    WeightedGraph inputGraph = WeightedGraph::fromFile(argv[1]);
    WeightedGraph minSpanningTree = inputGraph.getMinimumSpanningTree();
    cout << "Minimum spanning tree weight: " << minSpanningTree.getWeight() << endl;

    return 0;
}
