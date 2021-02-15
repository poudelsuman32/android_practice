#include <bits/stdc++.h>
#include <iostream>

using namespace std;
using pii = pair<int, int>;

const int N = 1e6;   // 10^6 cities limit
const int INF = 1e9; // Maximum distance limit

map<string, int> cityCode;
map<int, string> cityName;
vector<pii> adjList[N];
int g[N]; // distance
int f[N]; // f-value [f(n) = g(n) + h(n)]
int h[N]; // heuristics
int parent[N];
vector<int> route;
set<int> nodesExpanded;

// Uniform cost search
void search(int src, int dest)
{
    // Initialize the distance array
    for (int i = 0; i < N; i++)
    {
        g[i] = INF;
        f[i] = INF;
    }

    // Initialize a min heap
    priority_queue<pii, vector<pii>, greater<pii>> pq;

    // Initiate the algorithm for the source node
    g[src] = 0;
    pq.push({src, g[src]});

    bool destinationFound = false;

    while (!pq.empty())
    {
        int u = pq.top().first;

        // Add the current top node to the expanded set
        nodesExpanded.insert(u);

        pq.pop();

        for (auto y : adjList[u])
        {
            int v = y.first;
            int w = y.second;

            if (g[v] > g[u] + w)
            {

                g[v] = g[u] + w;

                if (!destinationFound)
                    pq.push({v, g[v]});

                // Track the node u that expands to v for route generation
                parent[v] = u;
            }
        }
    }
}

void getRoads()
{
    int cityCount = 0;
    string city1, city2;
    int roadLength;

    while (cin >> city1 >> city2 and (city1 != "END" and city2 != "OF"))
    {
        cin >> roadLength;

        // Map new cities to numbers and vice versa
        if (cityCode[city1] == 0)
        {
            cityCode[city1] = ++cityCount;
            cityName[cityCode[city1]] = city1;
        }

        if (cityCode[city2] == 0)
        {
            cityCode[city2] = ++cityCount;
            cityName[cityCode[city2]] = city2;
        }

        adjList[cityCode[city1]].push_back({cityCode[city2], roadLength});
        adjList[cityCode[city2]].push_back({cityCode[city1], roadLength});
    }
}

void getHeuristics()
{
    string city;
    int heuristicsValue;

    while (cin >> city and city != "END")
    {
        cin >> heuristicsValue;
        h[cityCode[city]] = heuristicsValue;
    }
}

void getRoute(int src, int city)
{
    if (city == src)
    {
        route.push_back(city);
        return;
    }
    else
    {
        getRoute(src, parent[city]);
        route.push_back(city);
    }
}

int main(int argc, char *argv[])
{
    // Process all roads into a graph
    freopen(argv[1], "r", stdin);
    getRoads();

    // Specify the source and destination nodes
    int sourceCity = cityCode[argv[2]];
    int destinationCity = cityCode[argv[3]];

    // Populate heuristics value array if it's an informed search
    if (argc == 5)
    {
        freopen(argv[4], "r", stdin);
        getHeuristics();
    }

    search(sourceCity, destinationCity);

    // Output
    cout << "nodes expanded: " << nodesExpanded.size() << endl;
    cout << "nodes generated: " << cityCode.size() << endl;

    if (g[destinationCity] == INF)
    {
        // There is no path between the given cities
        cout << "distance: infinity" << endl;
        cout << "route:" << endl
             << "none" << endl;
    }
    else
    {
        // There is a path between the given cities
        cout.precision(1);
        cout << "distance: " << fixed << (double)g[destinationCity] << " km" << endl;

        // Find the route recursively and show output
        getRoute(sourceCity, destinationCity);
        cout << "route:" << endl;
        for (int i = 0; i + 1 < route.size(); i++)
            cout << cityName[route[i]] << " to " << cityName[route[i + 1]] << ", " << fixed << (double)(g[route[i + 1]] - g[route[i]]) << " km" << endl;
    }
}