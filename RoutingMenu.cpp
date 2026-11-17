#include "RoutingMenu.h"
#include <iostream>
#include "ConsoleUtils.h"
#include "Routing.h"
using namespace std;

bool mapIsUsable(const Graph& graph) {
    if (graph.locationCount() == 0) {
        cout << "The map is empty. Add locations and roads first.\n";
        return false;
    }
    return true;
}

void askSimpleRoute(const Graph& graph, RouteMode mode) {
    if (!mapIsUsable(graph)) {
        return;
    }
    graph.printLocations(cout);
    int start = readInt("Start location ID: ");
    int end = readInt("Destination location ID: ");
    printRoute(graph, findRoute(graph, start, end, mode), cout);
}

void askViaStop(const Graph& graph) {
    if (!mapIsUsable(graph)) {
        return;
    }
    graph.printLocations(cout);
    int start = readInt("Start location ID: ");
    int stop = readInt("Required stop location ID: ");
    int end = readInt("Destination location ID: ");
    int choice = readIntInRange("Minimise  1) distance  2) travel time : ", 1, 2);
    RouteMode mode = ROUTE_TIME;
    if (choice == 1) {
        mode = ROUTE_DISTANCE;
    }
    printRoute(graph, routeViaStop(graph, start, stop, end, mode), cout);
}

void runRoutingMenu(const Graph& graph) {
    while (true) {
        printTitle("ROUTING");
        cout << " 1. Shortest route (minimum distance)\n"
             << " 2. Fastest route (minimum travel time)\n"
             << " 3. Route through one required stop\n"
             << " 0. Back to main menu\n";
        int choice = readIntInRange("Choice: ", 0, 3);

        if (choice == 0) {
            return;
        } else if (choice == 1) {
            askSimpleRoute(graph, ROUTE_DISTANCE);
        } else if (choice == 2) {
            askSimpleRoute(graph, ROUTE_TIME);
        } else {
            askViaStop(graph);
        }
    }
}
