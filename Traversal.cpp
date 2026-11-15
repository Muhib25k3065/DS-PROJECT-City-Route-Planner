#include "Traversal.h"
#include "LinkedQueue.h"
using namespace std;

bool checkStart(const Graph& graph, int startID, string& error) {
    if (graph.locationCount() == 0) {
        error = "The map is empty. Add locations and roads first.";
        return false;
    }
    if (!graph.isValidLocation(startID)) {
        error = "Invalid start location ID.";
        return false;
    }
    return true;
}

bool bfsReachable(const Graph& graph, int startID, vector<int>& reachable, string& error) {
    reachable.clear();
    if (!checkStart(graph, startID, error)) {
        return false;
    }

    vector<bool> visited(graph.capacity(), false);
    LinkedQueue<int> q;
    visited[startID] = true;
    q.enqueue(startID);

    while (!q.isEmpty()) {
        int u = -1;
        q.dequeue(u);
        reachable.push_back(u);
        for (const RoadNode* edge = graph.getOutgoing(u); edge != NULL; edge = edge->next) {
            if (!edge->isOpen) {
                continue;
            }
            int v = edge->toID;
            if (!visited[v]) {
                visited[v] = true;
                q.enqueue(v);
            }
        }
    }
    return true;
}

RouteResult bfsFewestRoads(const Graph& graph, int startID, int endID) {
    RouteResult result;
    if (!checkStart(graph, startID, result.message)) {
        return result;
    }
    if (!graph.isValidLocation(endID)) {
        result.message = "Invalid destination location ID.";
        return result;
    }
    if (startID == endID) {
        result.success = true;
        result.path.push_back(startID);
        result.message = "Start and destination are the same location.";
        return result;
    }

    int n = graph.capacity();
    vector<bool> visited(n, false);
    vector<int> parent(n, -1);
    vector<double> parentDistance(n, 0.0);
    vector<double> parentTime(n, 0.0);
    LinkedQueue<int> q;

    visited[startID] = true;
    q.enqueue(startID);
    bool found = false;

    while (!q.isEmpty() && !found) {
        int u = -1;
        q.dequeue(u);
        for (const RoadNode* edge = graph.getOutgoing(u); edge != NULL; edge = edge->next) {
            if (!edge->isOpen) {
                continue;
            }
            int v = edge->toID;
            if (visited[v]) {
                continue;
            }
            visited[v] = true;
            parent[v] = u;
            parentDistance[v] = edge->distance;
            parentTime[v] = edge->travelTime;
            if (v == endID) {
                found = true;
                break;
            }
            q.enqueue(v);
        }
    }

    if (!found) {
        result.message = "No route exists: the destination cannot be reached "
                         "(closed roads or one-way restrictions).";
        return result;
    }

    vector<int> backwards;
    for (int v = endID; v != -1; v = parent[v]) {
        backwards.push_back(v);
        if (v != startID) {
            result.totalDistance += parentDistance[v];
            result.totalTime += parentTime[v];
        }
    }
    int total = backwards.size();
    for (int i = total - 1; i >= 0; i--) {
        result.path.push_back(backwards[i]);
    }
    result.success = true;
    result.message = "Route with the fewest roads found.";
    return result;
}
