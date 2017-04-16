//
//  main.cpp
//  assignment-two
//
//  Created by Josef Dolezal on 14/04/2017.
//  Copyright © 2017 Josef Dolezal. All rights reserved.
//

#include <iostream>
#include <assert.h>
#include <vector>

using namespace std;

/**
 Represents point in 3D space. May also be used to define area size.
 */
struct Coordinates {
    int x;
    int y;
    int z;

    Coordinates(int x, int y, int z): x(x), y(y), z(z) {}

    Coordinates(): x(-1), y(-1), z(-1) {}
};

ostream & operator << (ostream & lhs, const Coordinates & rhs) {
    lhs << "(" << rhs.x << ", " << rhs.y << ", " << rhs.z << ")";

    return lhs;
}

bool operator == (const Coordinates & lhs, const Coordinates & rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

/**
 Edge from `owner` to destination. Keeps track of current flow.
 */
struct Edge {
    int flow;
    int destination;
    int start;

    Edge(int flow, int start, int destination):
        flow(flow), start(start), destination(destination) {}
};

/**
 Graph node with list of neighbors.
 */
struct Cube {
    vector<Edge> neighbors;
    Coordinates coords;

    Cube() {}
};

/**
 Wrapper for basic graph info. Defines how to read input data to create new graph.
 */
struct GraphAttributes {
    Coordinates size;
    int infectedCubes;

    GraphAttributes(Coordinates size, int infectedCubes): size(size), infectedCubes(infectedCubes) {}
};

/**
 Reads graph attributes from standard input.

 @return New object with given graph attributes
 */
Coordinates readGraphAttributes();

/**
 Reads area coordinates from standard input.

 @return New object representing readed coordinates
 */
Coordinates readCoordinates();

/**
 Reads list of infected cubes and stores them as neighbors of given start.

 @param graph Graph where infected cubes will be written to
 @param size Size system of given graph
 @return Modified graph with infected cubes as neighbors to start
 */
Cube * readInfectedCubes(Cube * graph, Coordinates size);

/**
 Reads list of special rooms and stores it in given graph. Special cubes are connected to given sink.
 */
/**
 Reads list of special cubes from standard input to given graph.

 @param graph Graph which where special cubes will be written to
 @param size Size system of given graph
 @return Modified graph with special cubes as neighbors to sink
 */
Cube * readSpecialCubes(Cube * graph, Coordinates size);

/**
 Creates new base on given attributes. In graph, the nodes have offset of 2 because
 of common start node at index 0 and common sink at index 1.

 @param attributes Basic informations about graph structure
 @return New graph created from standard data input with format defined by given attributes
 */
Cube * createGraph(Coordinates attributes);

/**
 Cleans resources allocated by given graph.

 @param graph Graph which resources will be deallocated
 */
void cleanup(Cube * graph);

/**
 Flattens 3D coordinates into one dimensional index in coordinates system.

 @param coordinates Coordinates of point in 3D space
 @param system Coordinates system on which is flattening based
 @return Flatten representation of coordnates
 */
int flattenCoordinates(const Coordinates & coordinates, const Coordinates & system);

/**
 Makes 3D coordinates from flat index in given coordinates system.

 @param index Index which should be expanded
 @param system System used for expanding calculations
 @return Coordinates of flat point in 3D space
 */
Coordinates expandIndex(int index, const Coordinates & system);

/**
 Determines whether the given coordinates are located on side wall
 in given graph (represented as square).

 @param coords Coordinations of which position will be checked
 @param system Graph size
 @return True if cube is on side wall, false otherwise
 */
bool isOnGraphSide(const Coordinates coords, const Coordinates & system);

/**
 Shorthand for graph global start.

 @param graph Graph where the start will be taken from
 @return Reference to global start cube
 */
Cube & graphStart(Cube * graph);

/**
 Shorthand for graph global sink.

 @param graph Graph where the sink will be taken from
 @return Reference to global sink
 */
Cube & graphSink(Cube * graph);

/**
 Shorthand for specific cube at given index.

 @param graph Graph where the cube will be taken from
 @param index Flat index of 3D coordinates representation
 @return Reference to cube at given index
 */
Cube & cubeAtIndex(Cube * graph, int index);

void setCoordinators(Cube & cube, int x, int y, int z) {
    cube.coords.x = x;
    cube.coords.y = y;
    cube.coords.z = z;
}

Cube * graphMinimalCovering(Cube * graph, Coordinates attributes);

bool augumentingPathExists(Cube * graph, vector<Edge *> & path);

bool dfs(Cube * graph, int start, int end, bool * visited, vector<Edge *> & path);

int main(int argc, const char * argv[]) {
    Coordinates attributes = readGraphAttributes();

    cleanup(
        graphMinimalCovering(
            readSpecialCubes(
                readInfectedCubes(
                    createGraph(attributes),
                    attributes),
                attributes),
            attributes)
    );

    return 0;
}

int flattenCoordinates(const Coordinates & coordinates, const Coordinates & system) {
    int floor = coordinates.y * system.x + coordinates.x;
    return floor + coordinates.z * system.x * system.y;
}

Coordinates expandIndex(int index, const Coordinates & system) {
    int floorSize = (system.x * system.y);
    int z = index / floorSize;
    index -= z * floorSize;

    int y = index / system.x;
    index -= y * system.x;

    return Coordinates(index, y, z);
}

Coordinates readGraphAttributes() {
    // Read coordinates must be increased by one
    Coordinates coords = readCoordinates();

    return Coordinates(coords.x + 1, coords.y + 1, coords.z + 1);
}

Coordinates readCoordinates() {
    int x, y, z;

    cin >> x >> y >> z;

    return Coordinates(x-1, y-1, z-1);
}

Cube * createGraph(Coordinates size) {
    // 0: global start, 1: global sink
    int index = 2;
    int nodeCount = size.x * size.y * size.z + 2;
    Cube * graph = new Cube[nodeCount];

    for(int z = 0; z < size.z; ++z)
        for (int y = 0; y < size.y; ++y)
            for (int x = 0; x < size.x; ++x) {
                Cube & cube = cubeAtIndex(graph, index);

                setCoordinators(cube, x, y, z);

                // Add all possible neighbors and flatten their index
                if(z + 1 < size.z)
                    cube.neighbors.push_back(Edge(1, index, 2 + flattenCoordinates(Coordinates(x, y, z+1), size)));
                if(z - 1 >= 0)
                    cube.neighbors.push_back(Edge(1, index, 2 + flattenCoordinates(Coordinates(x, y, z-1), size)));
                if(y + 1 < size.y)
                    cube.neighbors.push_back(Edge(1, index, 2 + flattenCoordinates(Coordinates(x, y+1, z), size)));
                if(y - 1 >= 0)
                    cube.neighbors.push_back(Edge(1, index, 2 + flattenCoordinates(Coordinates(x, y-1, z), size)));
                if(x + 1 < size.x)
                    cube.neighbors.push_back(Edge(1, index, 2 + flattenCoordinates(Coordinates(x+1, y, z), size)));
                if(x - 1 >= 0)
                    cube.neighbors.push_back(Edge(1, index, 2 + flattenCoordinates(Coordinates(x-1, y, z), size)));

                // Add global sink as neighbor if cube is on side of graph
                if(isOnGraphSide(Coordinates(x, y, z), size)) {
                    cube.neighbors.push_back(Edge(INT_MAX, index, 1));
                }

                ++index;
            }

    return graph;
}

bool augumentingPathExists(Cube * graph, vector<Edge *> & path, int size) {
    bool visited[size];
    for(int i = 0; i < size; ++i) visited[i] = false;

    return dfs(graph, 0, 1, visited, path);
}

bool dfs(Cube * graph, int start, int end, bool * visited, vector<Edge *> & path) {
    visited[start] = true;

    if(start == end) {
        return true;
    }

    Cube & node = graph[start];

    for(auto & neighbor: node.neighbors) {
        if(neighbor.flow > 0 && !visited[neighbor.destination]) {
            if(dfs(graph, neighbor.destination, end, visited, path)) {
                path.push_back(&neighbor);

                return true;
            }
        }
    }

    return false;
}

void findReachable(Cube * graph, int start, int end, bool * visited) {
    visited[start] = true;

    Cube & node = cubeAtIndex(graph, start);

    for (auto & neighbor: node.neighbors) {
        if(neighbor.flow > 0 && !visited[neighbor.destination]) {
            findReachable(graph, neighbor.destination, end, visited);
        }
    }
}

Cube * graphMinimalCovering(Cube * graph, Coordinates attributes) {
    int size = attributes.x * attributes.y * attributes.z + 2;
    vector<Edge *> path;
    int space = 0;

    while (augumentingPathExists(graph, path, size)) {
        int pathFlow = INT_MAX;

        for(auto edge: path) {
            cout << edge->destination << " <- ";
            pathFlow = min(pathFlow, edge->flow);
        }

        cout << endl;

        // Decrease the flow in one direction and increase it in opposite
        for(auto edge: path) {
            Cube & oposite = cubeAtIndex(graph, edge->destination);

            if(edge->start != 0)
                edge->flow -= pathFlow;

            for(auto & opositeEdge: oposite.neighbors) {
                if(opositeEdge.destination == edge->start) {
                    opositeEdge.flow += pathFlow;
                    break;
                }
            }
        }

        cout << pathFlow << endl;

        space += pathFlow;
        path.clear();
    }

    cout << space << endl;

    bool visited[size];
    for (int i = 0; i < size; ++i) visited[i] = false;

    findReachable(graph, 0, 1, visited);

    for (int i = 0; i < size; ++i) {
        if (visited[i] && i > 1)
            cout << "saturated: " << expandIndex(i-2, attributes) << endl;
    }

    for (int i = 2; i < size; ++i) {
        Cube & current = cubeAtIndex(graph, i);

        for(Edge & neighbor: current.neighbors) {
            if(visited[i] && !visited[neighbor.destination]) {
                cout << neighbor.start << ", " << neighbor.destination << "," << neighbor.flow;
            }
        }
    }

    return graph;
}

void cleanup(Cube * graph) {
    delete[] graph;
}

Cube * readInfectedCubes(Cube * graph, Coordinates size) {
    Cube & start = graphStart(graph);
    int count;

    cin >> count;

    for (int i = 0; i < count; ++i) {
        Coordinates coords = readCoordinates();

        int infectedCube = flattenCoordinates(coords, size) + 2;

        cout << coords.x << " " << coords.y << coords.z << endl;
        cout << flattenCoordinates(coords, size) << endl;

        // Add edge from start to infected cube
        start.neighbors.push_back(Edge(INT_MAX, 0, infectedCube));
    }
    return graph;
}

Cube * readSpecialCubes(Cube * graph, Coordinates size) {
    int count;

    cin >> count;

    for (int i = 0; i < count; ++i) {
        Coordinates coords = readCoordinates();
        int special = flattenCoordinates(coords, size) + 2;

        // Add edge from special cube to the sink
        if(!isOnGraphSide(coords, size)) {
            graph[special].neighbors.push_back(Edge(INT_MAX, special, 1));
        }
    }
    return  graph;
}

bool isOnGraphSide(const Coordinates coords, const Coordinates & system) {
    return coords.x == 0 || coords.y == 0 || coords.z == 0 || coords.x + 1 == system.x
    || coords.y + 1 == system.y || coords.z + 1 == system.z;
}

Cube & graphStart(Cube * graph) {
    return cubeAtIndex(graph, 0);
}

Cube & graphSink(Cube * graph) {
    return cubeAtIndex(graph, 1);
}

Cube & cubeAtIndex(Cube * graph, int index) {
    return graph[index];
}
