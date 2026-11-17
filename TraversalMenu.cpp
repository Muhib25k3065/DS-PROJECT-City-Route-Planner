#include "TraversalMenu.h"
#include <iostream>
#include "ConsoleUtils.h"
#include "Routing.h"
#include "Traversal.h"
using namespace std;

void showReachable(const Graph& graph) {
    if (graph.locationCount() == 0) {
        cout << "The map is empty. Add locations and roads first.\n";
        return;
    }
    graph.printLocations(cout);
    int start = readInt("Start location ID: ");
    vector<int> reachable;
    string error;
    if (!bfsReachable(graph, start, reachable, error)) {
        cout << "Error: " << error << "\n";
        return;
    }
    cout << "Reachable from " << graph.nameOf(start) << " (in BFS order):\n";
    int total = reachable.size();
    for (int i = 0; i < total; i++) {
        cout << "  " << graph.nameOf(reachable[i]) << "\n";
    }
    cout << (total - 1) << " other location(s) can be reached.\n";
}

void showFewestRoads(const Graph& graph) {
    if (graph.locationCount() == 0) {
        cout << "The map is empty. Add locations and roads first.\n";
        return;
    }
    graph.printLocations(cout);
    int start = readInt("Start location ID: ");
    int end = readInt("Destination location ID: ");
    printRoute(graph, bfsFewestRoads(graph, start, end), cout);
    cout << "(BFS minimises the NUMBER of roads, not distance or time.)\n";
}

void runTraversalMenu(const Graph& graph) {
    while (true) {
        printTitle("BFS TRAVERSAL");
        cout << " 1. Show reachable locations\n"
             << " 2. Route with the fewest roads\n"
             << " 0. Back to main menu\n";
        int choice = readIntInRange("Choice: ", 0, 2);

        if (choice == 0) {
            return;
        } else if (choice == 1) {
            showReachable(graph);
        } else {
            showFewestRoads(graph);
        }
    }
}
