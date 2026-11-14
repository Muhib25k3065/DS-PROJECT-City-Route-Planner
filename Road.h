#ifndef ROAD_H
#define ROAD_H

#include <cstddef>
#include <string>
using namespace std;

struct Road {
    int roadID;
    int fromID;
    int toID;
    double distance;
    double travelTime;
    bool isOpen;
    bool isTwoWay;

    Road() {
        roadID = -1;
        fromID = -1;
        toID = -1;
        distance = 0.0;
        travelTime = 0.0;
        isOpen = true;
        isTwoWay = false;
    }

    Road(int id, int from, int to, double dist, double time, bool open, bool twoWay) {
        roadID = id;
        fromID = from;
        toID = to;
        distance = dist;
        travelTime = time;
        isOpen = open;
        isTwoWay = twoWay;
    }
};

struct RoadNode {
    int roadID;
    int fromID;
    int toID;
    double distance;
    double travelTime;
    bool isOpen;
    bool isTwoWay;
    RoadNode* next;

    RoadNode(int id, int from, int to, double dist, double time, bool open, bool twoWay) {
        roadID = id;
        fromID = from;
        toID = to;
        distance = dist;
        travelTime = time;
        isOpen = open;
        isTwoWay = twoWay;
        next = NULL;
    }
};

bool validateRoadWeights(double distance, double travelTime, string& error);

Road nodeToRoad(const RoadNode& node);

RoadNode* createNode(const Road& road, bool reversed);

string roadToText(const Road& road);

#endif
