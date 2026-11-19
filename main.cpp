#include <iostream>
#include "ConsoleUtils.h"
#include "Graph.h"
#include "MapMenu.h"
#include "RoadMenu.h"
#include "RoutingMenu.h"
#include "TraversalMenu.h"
#include "UndoManager.h"
using namespace std;

int main() {
    Graph graph;
    UndoManager undo;

    cout << "CITY ROUTE PLANNER\n"
         << "Tip: load the demo map with  6 (Files) -> 2 (Load) -> data/sample_map.txt\n";

    while (true) {
        printTitle("MAIN MENU");
        cout << " 1. Locations\n"
             << " 2. Roads\n"
             << " 3. Routing (shortest / fastest / required stop)\n"
             << " 4. BFS (reachable locations / fewest roads)\n"
             << " 5. Undo latest road change\n"
             << " 6. Files (save / load)\n"
             << " 7. Exit\n";
        int choice = readIntInRange("Choice: ", 1, 7);

        if (choice == 1) {
            runLocationMenu(graph, undo);
        } else if (choice == 2) {
            runRoadMenu(graph, undo);
        } else if (choice == 3) {
            runRoutingMenu(graph);
        } else if (choice == 4) {
            runTraversalMenu(graph);
        } else if (choice == 5) {
            runUndoAction(graph, undo);
        } else if (choice == 6) {
            runFileMenu(graph, undo);
        } else {
            cout << "Goodbye! (All memory is released automatically.)\n";
            return 0;
        }
    }
}
