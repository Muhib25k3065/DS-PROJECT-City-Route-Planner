#include "Road.h"
#include <iomanip>
#include <sstream>
using namespace std;

bool validateRoadWeights(double distance, double travelTime, string& error) {
    if (distance != distance || travelTime != travelTime) {
        error = "Distance and travel time must be valid numbers.";
        return false;
    }
    if (distance < 0.0) {
        error = "Distance cannot be negative.";
        return false;
    }
    if (travelTime < 0.0) {
        error = "Travel time cannot be negative.";
        return false;
    }
    return true;
}

Road nodeToRoad(const RoadNode& node) {
    Road road(node.roadID, node.fromID, node.toID, node.distance,
              node.travelTime, node.isOpen, node.isTwoWay);
    return road;
}

RoadNode* createNode(const Road& road, bool reversed) {
    int from = road.fromID;
    int to = road.toID;
    if (reversed) {
        from = road.toID;
        to = road.fromID;
    }
    RoadNode* node = new RoadNode(road.roadID, from, to, road.distance,
                                  road.travelTime, road.isOpen, road.isTwoWay);
    return node;
}

string roadToText(const Road& road) {
    ostringstream out;
    out << fixed << setprecision(2);
    out << "Road #" << road.roadID << ": " << road.fromID;
    if (road.isTwoWay) {
        out << " <-> ";
    } else {
        out << " -> ";
    }
    out << road.toID << " (" << road.distance << " km, " << road.travelTime << " min, ";
    if (road.isOpen) {
        out << "OPEN";
    } else {
        out << "CLOSED";
    }
    out << ")";
    return out.str();
}
