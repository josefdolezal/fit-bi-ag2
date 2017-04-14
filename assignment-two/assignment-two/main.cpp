//
//  main.cpp
//  assignment-two
//
//  Created by Josef Dolezal on 14/04/2017.
//  Copyright © 2017 Josef Dolezal. All rights reserved.
//

#include <iostream>
#include <assert.h>

using namespace std;

/**
 Represents point in 3D space. May also be used to define area size.
 */
struct Coordinates {
    int x;
    int y;
    int z;

    Coordinates(int x, int y, int z): x(x), y(y), z(z) {}
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
GraphAttributes readGraphAttributes();

/**
 Creates new base on given attributes.

 @param attributes Basic informations about graph structure
 @return New graph created from standard data input with format defined by given attributes
 */
int * createGraph(GraphAttributes attributes);

/**
 Cleans resources allocated by given graph.

 @param graph Graph which resources will be deallocated
 */
void cleanup(int * graph);

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

int main(int argc, const char * argv[]) {

    cleanup(createGraph(readGraphAttributes()));

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

GraphAttributes readGraphAttributes() {
    int x, y, z, infectedCubes;

    cin >> x >> y >> z >> infectedCubes;

    return GraphAttributes(Coordinates(x, y ,z), infectedCubes);
}

int * createGraph(GraphAttributes attributes) {
    int nodeCount = attributes.size.x * attributes.size.y * attributes.size.z + 2;
    int * graph = new int[nodeCount];

    for(int i = 0; i < attributes.infectedCubes; ++i) {

    }

    return graph;
}

void cleanup(int * graph) {
    delete graph;
}
