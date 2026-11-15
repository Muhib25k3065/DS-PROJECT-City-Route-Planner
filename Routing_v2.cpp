#include "Routing.h"
#include "MinHeap.h"
#include <cfloat>
#include <iomanip>
#include <sstream>
using namespace std;

bool checkSearchInputs(const Graph& graph, int startID, int endID, RouteResult& result) {
    if (graph.locationCount() == 0) {
        result.message = "The map is empty. Add locations and roads first.";
        return false;
    }
    if (!graph.isValidLocation(startID)) {
        result.message = "Invalid start location ID.";
        return false;
    }
    if (!graph.isValidLocation(endID)) {
        result.message = "Invalid destination location ID.";
        return false;
    }
    return true;
}

vector<int> reconstructPath(const vector<int>& parent, int startID, int endID) {
    vector<int> backwards;
    int current = endID;
    int steps = 0;
    int limit = parent.size() + 1;

    while (current != -1 && steps < limit) {
        backwards.push_back(current);
        if (current == startID) {
            break;
        }
        current = parent[current];
        steps++;
    }

    vector<int> path;
    int total = backwards.size();
    for (int i = total - 1; i >= 0; i--) {
        path.push_back(backwards[i]);
    }

    if (path.empty() || path[0] != startID) {
        path.clear();
    }
    return path;
}

RouteResult findRoute(const Graph& graph, int startID, int endID, RouteMode mode) {
    RouteResult result;
    if (!checkSearchInputs(graph, startID, endID, result)) {
        return result;
    }
    if (startID == endID) {
        result.success = true;
        result.path.push_back(startID);
        result.message = "Start and destination are the same location.";
        return result;
    }

    int n = graph.capacity();
    double INF = DBL_MAX;

    vector<double> dist(n, INF);
    vector<int> parent(n, -1);
    vector<double> parentDistance(n, 0.0);
    vector<double> parentTime(n, 0.0);
    vector<bool> settled(n, false);
    MinHeap heap(n);

    dist[startID] = 0.0;
    heap.insert(startID, 0.0);

    while (!heap.isEmpty()) {
        int u = -1;
        double key = 0.0;
        heap.extractMin(u, key);
        settled[u] = true;
        if (u == endID) {
            break;
        }

        for (const RoadNode* edge = graph.getOutgoing(u); edge != NULL; edge = edge->next) {
            if (!edge->isOpen) {
                continue;
            }
            int v = edge->toID;
            if (settled[v]) {
                continue;
            }
            double weight = edge->travelTime;
            if (mode == ROUTE_DISTANCE) {
                weight = edge->distance;
            }
            double candidate = dist[u] + weight;
            if (candidate < dist[v]) {
                dist[v] = candidate;
                parent[v] = u;
                parentDistance[v] = edge->distance;
                parentTime[v] = edge->travelTime;
                if (heap.contains(v)) {
                    heap.decreaseKey(v, candidate);
                } else {
                    heap.insert(v, candidate);
                }
            }
        }
    }

    if (!settled[endID]) {
        result.message = "No route exists: the destination cannot be reached "
                         "(closed roads or one-way restrictions).";
        return result;
    }

    result.path = reconstructPath(parent, startID, endID);
    for (int v = endID; v != startID; v = parent[v]) {
        result.totalDistance += parentDistance[v];
        result.totalTime += parentTime[v];
    }
    result.success = true;
    if (mode == ROUTE_DISTANCE) {
        result.message = "Shortest-distance route found.";
    } else {
        result.message = "Fastest route found.";
    }
    return result;
}

void printRoute(const Graph& graph, const RouteResult& result, ostream& os) {
    if (!result.success) {
        os << "No route: " << result.message << "\n";
        return;
    }
    os << result.message << "\n";

    int pathSize = result.path.size();
    int roadsUsed = 0;
    if (pathSize > 0) {
        roadsUsed = pathSize - 1;
    }
    os << "  Roads used: " << roadsUsed << "\n";

    os << "  Path: ";
    for (int i = 0; i < pathSize; i++) {
        if (i > 0) {
            os << " -> ";
        }
        os << graph.nameOf(result.path[i]);
    }
    os << "\n";

    ostringstream totals;
    totals << fixed << setprecision(2);
    totals << "  Total distance: " << result.totalDistance << " km\n";
    totals << "  Total time:     " << result.totalTime << " min\n";
    os << totals.str();
}

RouteResult routeViaStop(const Graph& graph, int startID, int stopID, int endID, RouteMode mode) {
    RouteResult result;
    if (graph.locationCount() == 0) {
        result.message = "The map is empty. Add locations and roads first.";
        return result;
    }
    if (!graph.isValidLocation(stopID)) {
        result.message = "Invalid required-stop location ID.";
        return result;
    }

    RouteResult firstLeg = findRoute(graph, startID, stopID, mode);
    if (!firstLeg.success) {
        result.message = "The required stop cannot be reached from the start. " + firstLeg.message;
        return result;
    }

    RouteResult secondLeg = findRoute(graph, stopID, endID, mode);
    if (!secondLeg.success) {
        result.message = "The destination cannot be reached from the required stop. " + secondLeg.message;
        return result;
    }

    result.path = firstLeg.path;
    int secondSize = secondLeg.path.size();
    for (int i = 1; i < secondSize; i++) {
        result.path.push_back(secondLeg.path[i]);
    }
    result.totalDistance = firstLeg.totalDistance + secondLeg.totalDistance;
    result.totalTime = firstLeg.totalTime + secondLeg.totalTime;
    result.success = true;
    result.message = "Route through the required stop found.";
    return result;
}
