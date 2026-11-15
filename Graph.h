#ifndef GRAPH_H
#define GRAPH_H

#include <ostream>
#include <string>
#include <vector>
#include "Location.h"
#include "Road.h"
using namespace std;

class Graph {
private:
    vector<Location> locations;
    vector<RoadNode*> adjacency;
    int nextRoadID;
    int activeCount;
    string lastError;

    void freeList(int id);
    RoadNode* findNode(int fromID, int toID) const;
    RoadNode* findNodeByRoadID(int roadID) const;
    bool checkNewRoad(const Road& road);
    void appendNode(RoadNode* node);
    void insertRoadNodes(const Road& road);

public:
    Graph();
    ~Graph();

    int addLocation(const string& name);
    bool editLocation(int id, const string& newName);
    bool isValidLocation(int id) const;
    const Location* getLocation(int id) const;
    string nameOf(int id) const;
    int findLocationByName(const string& name) const;
    int capacity() const;
    int locationCount() const;

    int addRoad(int fromID, int toID, double distance, double travelTime, bool twoWay);
    bool getRoad(int roadID, Road& out) const;
    const RoadNode* getOutgoing(int id) const;
    int roadCount() const;

    void clear();
    void printLocations(ostream& os) const;
    void printMap(ostream& os) const;
    const string& getLastError() const;
};

#endif
