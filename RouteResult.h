#ifndef ROUTERESULT_H
#define ROUTERESULT_H

#include <string>
#include <vector>
using namespace std;

enum RouteMode {
    ROUTE_DISTANCE,
    ROUTE_TIME
};

struct RouteResult {
    bool success;
    vector<int> path;
    double totalDistance;
    double totalTime;
    string message;

    RouteResult() {
        success = false;
        totalDistance = 0.0;
        totalTime = 0.0;
    }
};

#endif
