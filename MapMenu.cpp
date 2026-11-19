#include "MapMenu.h"
#include <iostream>
#include "ConsoleUtils.h"
#include "FileManager.h"
using namespace std;

void addLocation(Graph& graph) {
    string name = readLine("New location name: ");
    int id = graph.addLocation(name);
    if (id < 0) {
        cout << "Error: " << graph.getLastError() << "\n";
    } else {
        cout << "Added \"" << graph.nameOf(id) << "\" with ID " << id << ".\n";
    }
}

void editLocation(Graph& graph) {
    if (graph.locationCount() == 0) {
        cout << "There are no locations to edit.\n";
        return;
    }
    graph.printLocations(cout);
    int id = readInt("ID of the location to rename: ");
    string name = readLine("New name: ");
    if (graph.editLocation(id, name)) {
        cout << "Location renamed.\n";
    } else {
        cout << "Error: " << graph.getLastError() << "\n";
    }
}

void deleteLocation(Graph& graph, UndoManager& undo) {
    if (graph.locationCount() == 0) {
        cout << "There are no locations to delete.\n";
        return;
    }
    graph.printLocations(cout);
    int id = readInt("ID of the location to delete: ");
    if (!graph.isValidLocation(id)) {
        cout << "Error: no location with ID " << id << " exists.\n";
        return;
    }
    if (!readYesNo("Delete \"" + graph.nameOf(id) + "\" and every road connected to it?")) {
        cout << "Cancelled.\n";
        return;
    }
    if (graph.deleteLocation(id)) {
        undo.clear();
        cout << "Location deleted. Undo history was cleared.\n";
    } else {
        cout << "Error: " << graph.getLastError() << "\n";
    }
}

void saveFile(const Graph& graph) {
    string name = readLine("File name to save to (e.g. data/my_map.txt): ");
    string error;
    if (saveMap(graph, name, error)) {
        cout << "Map saved to \"" << name << "\".\n";
    } else {
        cout << "Error: " << error << "\n";
    }
}

void loadFile(Graph& graph, UndoManager& undo) {
    string name = readLine("File name to load (e.g. data/sample_map.txt): ");
    string error;
    if (loadMap(graph, name, error)) {
        undo.clear();
        cout << "Map loaded: " << graph.locationCount() << " location(s), "
             << graph.roadCount() << " road(s). Undo history was cleared.\n";
    } else {
        cout << "Load failed: " << error << "\nThe current map was not changed.\n";
    }
}

void runLocationMenu(Graph& graph, UndoManager& undo) {
    while (true) {
        printTitle("LOCATIONS");
        cout << " 1. Add location\n"
             << " 2. Edit (rename) location\n"
             << " 3. Delete location\n"
             << " 4. Display map\n"
             << " 0. Back to main menu\n";
        int choice = readIntInRange("Choice: ", 0, 4);

        if (choice == 0) {
            return;
        } else if (choice == 1) {
            addLocation(graph);
        } else if (choice == 2) {
            editLocation(graph);
        } else if (choice == 3) {
            deleteLocation(graph, undo);
        } else if (choice == 4) {
            graph.printMap(cout);
        }
    }
}

void runFileMenu(Graph& graph, UndoManager& undo) {
    while (true) {
        printTitle("FILES");
        cout << " 1. Save map to a text file\n"
             << " 2. Load map from a text file\n"
             << " 0. Back to main menu\n";
        int choice = readIntInRange("Choice: ", 0, 2);

        if (choice == 0) {
            return;
        } else if (choice == 1) {
            saveFile(graph);
        } else {
            loadFile(graph, undo);
        }
    }
}
