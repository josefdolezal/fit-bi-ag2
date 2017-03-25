//
//  main.cpp
//  assignment-one
//
//  Created by Josef Dolezal on 20/03/2017.
//  Copyright © 2017 Josef Dolezal. All rights reserved.
//

#include <iostream>
#include <vector>

using namespace std;

/**
 Represents graph node
 */
struct City;

/**
 Reads single number from standrd input

 @return Signle number represeinting number of nodes in graph
 */
int getMapSize();

/**
 Creates new graph with given number of nodes

 @param mapSize Total count of
 @return New graph containing only nodes, edges must be added explicitly
 */
City * createMap(const int mapSize);

/**
 Reads graph edges from standard input

 @param map Graph, where edges will be added to
 @param mapSize Size of graph (number of vertexes)
 @return Modified graph with edges
 */
City * loadStreets(City * map, const int mapSize);

/**
 Searches for useless edges and removes them, uses dfs algorithm under the hood

 @param map Graph where should be removed duplicate edges
 @return Modified graph with useless edges marked as not displayable
 */
City * simplifyMap(City * map);

/**
 Iterates over graph nodes and searches for useless edges

 @param map Graph which will be searched
 @param time Global opening timestamp, marks when each node was opened
 @param node ID of node where will DFS start
 @param parent ID of current node, set -1 if node does not have parent
 */
void dfs(City * map, int & time, int node, int parent);

/**
 Iterates over graph and prints for each node it's neighbor vertexes

 @param map Graph to be displayed
 @param mapSize Size of graph to be able to iterate through it
 @return Unmodified graph
 */
City * display(City * map, int mapSize);

/**
 Checks if given graph is valid to display

 @param map Graph to be displayed
 @param mapSize Size of graph to be able to iterate through it
 @return True if graph is displayable, false otherwise
 */
bool validate(City * map, int mapSize);

/**
 Takes care of correct graph memory cleanup

 @param map Graph to be cleaned from memory
 */
void cleanup(City * map);

/**
 Returns minimum of two positive numbers, treats -1 as infinity

 @param lhs First positive number
 @param rhs Second positive number
 @return Minimum of two positive numbers, if first is infinity, the second one is returned
 */
int minWithInfinity(int lhs, int rhs);

struct City {
public:
    int visitedAt = -1;

    int closestNeighbor = -1;

    int displayableEdges = 0;

    bool isVisited() { return visitedAt != -1; }

    bool hasClosestNeighbor() { return closestNeighbor != -1; }

    vector<int> neighbors = vector<int>();
};

int main() {
    int mapSize = getMapSize();

    cleanup(display(simplifyMap(loadStreets(createMap(mapSize), mapSize)), mapSize));

    return 0;
}

int getMapSize() {
    int mapSize;

    cin >> mapSize;

    return mapSize;
}

City * createMap(const int mapSize) {
    City * map = new City[mapSize];

    return map;
}

City * loadStreets(City * map, const int mapSize) {
    for(int start = 0; start < mapSize; ++start) {
        int streetsCount;

        cin >> streetsCount;
        map[start].displayableEdges = streetsCount;

        for(int iterator = 0; iterator < streetsCount; ++iterator) {
            int destination;

            cin >> destination;

            map[start].neighbors.push_back(destination);
        }
    }

    return map;
}

City * simplifyMap(City * map) {
    int openedAt = 0;

    dfs(map, openedAt, 0, -1);

    return map;
}

void dfs(City * map, int & time, int node, int parent) {
    City & currentCity = map[node];

    time += 1;
    currentCity.visitedAt = time;

    // cout << "opening node " << node << " with parent " << parent << endl;

    for(int neighborsIterator = 0; neighborsIterator < currentCity.neighbors.size(); ++neighborsIterator) {
        int neighbor = currentCity.neighbors[neighborsIterator];
        City & cityNeighbor = map[neighbor];

        // Tree edge
        if(!cityNeighbor.isVisited()) {
            dfs(map, time, neighbor, node);

            if(!cityNeighbor.hasClosestNeighbor() || cityNeighbor.closestNeighbor >= cityNeighbor.visitedAt) {
                // cout << node << " -> " << neighbor << " is a bridge" << endl;
                // cout << "    because neib's neib " << !cityNeighbor.hasClosestNeighbor() << " or " << cityNeighbor.closestNeighbor << " > " << cityNeighbor.visitedAt << endl;
            }

            currentCity.closestNeighbor = minWithInfinity(currentCity.closestNeighbor,
                                              cityNeighbor.closestNeighbor);
        }
        // Back edge
        else if(neighbor != parent && cityNeighbor.visitedAt < currentCity.visitedAt) {
            // cout << node << " -> " << neighbor << " is a back edge" << endl;
            // cout << "    min(" << currentCity.closestNeighbor << ", " << cityNeighbor.visitedAt << ") = " << minWithInfinity(currentCity.closestNeighbor, cityNeighbor.visitedAt) << endl;
            currentCity.closestNeighbor = minWithInfinity(currentCity.closestNeighbor, cityNeighbor.visitedAt);
        }
        else {
            // Mark edge as not displayable, decrement total count of displayable edges
            currentCity.displayableEdges -= 1;
            currentCity.neighbors[neighborsIterator] = -1;
        }
    }
}

City * display(City * map, int mapSize) {
    if(!validate(map, mapSize)) {
        cout << "No solution." << endl;
        return map;
    }

    for(int city = 0; city < mapSize; ++city) {
        int neighborsCount = (int) map[city].neighbors.size();
        int displayableNeighbors = map[city].displayableEdges;

        // Output 0 if no display
        if(map[city].displayableEdges <= 0) {
            cout << 0 << endl;
            continue;
        }

        cout << displayableNeighbors;

        for(int neighbor = 0; neighbor < neighborsCount; ++neighbor) {
            // Skip nondisplayable edges
            if(map[city].neighbors[neighbor] == -1) continue;

            cout << " " << map[city].neighbors[neighbor];
        }

        cout << endl;
    }

    return map;
}

bool validate(City * map, int mapSize) {
    for(int i = 0; i < mapSize; ++i)
        if(!map[i].hasClosestNeighbor()) return false;

    return true;
}

void cleanup(City * map) {
    delete [] map;
}

int minWithInfinity(int lhs, int rhs) {
    if(lhs < 0) return rhs;
    if(rhs < 0) return lhs;

    return min(lhs, rhs);
}
