#include "RoadMenu.h"
#include <iostream>
#include "ConsoleUtils.h"
using namespace std;

void reportFailure(const Graph& graph) {
    cout << "Error: " << graph.getLastError() << "\n";
}

bool askExistingRoad(const Graph& graph, Road& road) {
    graph.printMap(cout);
    int roadID = readInt("Road ID (the number after #): ");
    if (!graph.getRoad(roadID, road)) {
        cout << "Error: no road with ID " << roadID << " exists.\n";
        return false;
    }
    return true;
}

void addRoad(Graph& graph, UndoManager& undo) {
    if (graph.locationCount() < 2) {
        cout << "Add at least two locations first.\n";
        return;
    }
    graph.printLocations(cout);
    int from = readInt("From location ID: ");
    int to = readInt("To location ID: ");
    double distance = readNonNegativeDouble("Distance (km): ");
    double time = readNonNegativeDouble("Travel time (minutes): ");
    bool twoWay = readYesNo("Two-way road?");
    int roadID = undo.addRoad(graph, from, to, distance, time, twoWay);
    if (roadID < 0) {
        reportFailure(graph);
    } else {
        cout << "Road #" << roadID << " added.\n";
    }
}

void editRoad(Graph& graph, UndoManager& undo) {
    Road road;
    if (!askExistingRoad(graph, road)) {
        return;
    }
    cout << "Current: " << roadToText(road) << "\n";
    double distance = readNonNegativeDouble("New distance (km): ");
    double time = readNonNegativeDouble("New travel time (minutes): ");
    if (undo.updateRoad(graph, road.roadID, distance, time)) {
        cout << "Road updated.\n";
    } else {
        reportFailure(graph);
    }
}

void trafficUpdate(Graph& graph, UndoManager& undo) {
    Road road;
    if (!askExistingRoad(graph, road)) {
        return;
    }
    cout << "Current: " << roadToText(road) << "\n";
    double time = readNonNegativeDouble("New travel time with traffic (minutes): ");
    if (undo.updateRoad(graph, road.roadID, road.distance, time)) {
        cout << "Traffic update applied. It affects the NEXT route search.\n";
    } else {
        reportFailure(graph);
    }
}

void changeOpenState(Graph& graph, UndoManager& undo, bool open) {
    Road road;
    if (!askExistingRoad(graph, road)) {
        return;
    }
    if (road.isOpen == open) {
        if (open) {
            cout << "That road is already open.\n";
        } else {
            cout << "That road is already closed.\n";
        }
        return;
    }
    if (undo.setRoadOpen(graph, road.roadID, open)) {
        if (open) {
            cout << "Road #" << road.roadID << " reopened.\n";
        } else {
            cout << "Road #" << road.roadID << " closed.\n";
        }
    } else {
        reportFailure(graph);
    }
}

void deleteRoad(Graph& graph, UndoManager& undo) {
    Road road;
    if (!askExistingRoad(graph, road)) {
        return;
    }
    if (!readYesNo("Delete " + roadToText(road) + "?")) {
        cout << "Cancelled.\n";
        return;
    }
    if (undo.removeRoad(graph, road.roadID)) {
        cout << "Road deleted.\n";
    } else {
        reportFailure(graph);
    }
}

void runRoadMenu(Graph& graph, UndoManager& undo) {
    while (true) {
        printTitle("ROADS");
        cout << " 1. Add road\n"
             << " 2. Edit road (distance and time)\n"
             << " 3. Traffic update (change travel time only)\n"
             << " 4. Close road\n"
             << " 5. Reopen road\n"
             << " 6. Delete road\n"
             << " 7. Display map\n"
             << " 0. Back to main menu\n";
        int choice = readIntInRange("Choice: ", 0, 7);

        if (choice == 0) {
            return;
        } else if (choice == 1) {
            addRoad(graph, undo);
        } else if (choice == 2) {
            editRoad(graph, undo);
        } else if (choice == 3) {
            trafficUpdate(graph, undo);
        } else if (choice == 4) {
            changeOpenState(graph, undo, false);
        } else if (choice == 5) {
            changeOpenState(graph, undo, true);
        } else if (choice == 6) {
            deleteRoad(graph, undo);
        } else if (choice == 7) {
            graph.printMap(cout);
        }
    }
}

void runUndoAction(Graph& graph, UndoManager& undo) {
    printTitle("UNDO");
    if (!undo.canUndo()) {
        cout << "Nothing to undo.\n";
        return;
    }
    cout << undo.describeNextUndo() << "\n";
    if (!readYesNo("Undo it?")) {
        cout << "Cancelled.\n";
        return;
    }
    string message;
    undo.undoLast(graph, message);
    cout << message << "\n";
}
