#include "Graph.h"
#include <iomanip>
#include <sstream>
using namespace std;

string intToText(int value) {
    ostringstream out;
    out << value;
    return out.str();
}

Graph::Graph() {
    nextRoadID = 0;
    activeCount = 0;
}

Graph::~Graph() {
    clear();
}

void Graph::freeList(int id) {
    RoadNode* current = adjacency[id];
    while (current != NULL) {
        RoadNode* nextNode = current->next;
        delete current;
        current = nextNode;
    }
    adjacency[id] = NULL;
}

void Graph::clear() {
    int total = adjacency.size();
    for (int i = 0; i < total; i++) {
        freeList(i);
    }
    adjacency.clear();
    locations.clear();
    nextRoadID = 0;
    activeCount = 0;
    lastError.clear();
}

const string& Graph::getLastError() const {
    return lastError;
}

bool Graph::isValidLocation(int id) const {
    int total = locations.size();
    return id >= 0 && id < total && locations[id].active;
}

const Location* Graph::getLocation(int id) const {
    if (!isValidLocation(id)) {
        return NULL;
    }
    return &locations[id];
}

string Graph::nameOf(int id) const {
    int total = locations.size();
    if (id < 0 || id >= total) {
        return "?";
    }
    if (!locations[id].active) {
        return "(deleted)";
    }
    return locations[id].name;
}

int Graph::capacity() const {
    return locations.size();
}

int Graph::locationCount() const {
    return activeCount;
}

int Graph::findLocationByName(const string& name) const {
    string wanted = trimText(name);
    int total = locations.size();
    for (int i = 0; i < total; i++) {
        if (locations[i].active && namesEqualIgnoreCase(locations[i].name, wanted)) {
            return i;
        }
    }
    return -1;
}

int Graph::addLocation(const string& rawName) {
    string error;
    if (!isValidLocationName(rawName, error)) {
        lastError = error;
        return -1;
    }
    string name = trimText(rawName);
    if (findLocationByName(name) != -1) {
        lastError = "A location named \"" + name + "\" already exists.";
        return -1;
    }
    int id = locations.size();
    locations.push_back(Location(id, name));
    adjacency.push_back(NULL);
    activeCount++;
    return id;
}

bool Graph::editLocation(int id, const string& newName) {
    if (!isValidLocation(id)) {
        lastError = "No location with ID " + intToText(id) + " exists.";
        return false;
    }
    string error;
    if (!isValidLocationName(newName, error)) {
        lastError = error;
        return false;
    }
    string name = trimText(newName);
    int existing = findLocationByName(name);
    if (existing != -1 && existing != id) {
        lastError = "A location named \"" + name + "\" already exists.";
        return false;
    }
    locations[id].name = name;
    return true;
}

RoadNode* Graph::findNode(int fromID, int toID) const {
    int total = adjacency.size();
    if (fromID < 0 || fromID >= total) {
        return NULL;
    }
    for (RoadNode* node = adjacency[fromID]; node != NULL; node = node->next) {
        if (node->toID == toID) {
            return node;
        }
    }
    return NULL;
}

RoadNode* Graph::findNodeByRoadID(int roadID) const {
    int total = adjacency.size();
    for (int i = 0; i < total; i++) {
        for (RoadNode* node = adjacency[i]; node != NULL; node = node->next) {
            if (node->roadID == roadID) {
                return node;
            }
        }
    }
    return NULL;
}

bool Graph::checkNewRoad(const Road& road) {
    if (!isValidLocation(road.fromID) || !isValidLocation(road.toID)) {
        lastError = "Both endpoints must be existing locations.";
        return false;
    }
    if (road.fromID == road.toID) {
        lastError = "A road must connect two different locations.";
        return false;
    }
    string error;
    if (!validateRoadWeights(road.distance, road.travelTime, error)) {
        lastError = error;
        return false;
    }
    if (findNode(road.fromID, road.toID) != NULL) {
        lastError = "A road from " + nameOf(road.fromID) + " to " + nameOf(road.toID) + " already exists.";
        return false;
    }
    if (road.isTwoWay && findNode(road.toID, road.fromID) != NULL) {
        lastError = "A road from " + nameOf(road.toID) + " to " + nameOf(road.fromID) +
                    " already exists, so a two-way road cannot be added.";
        return false;
    }
    return true;
}

void Graph::appendNode(RoadNode* node) {
    RoadNode* head = adjacency[node->fromID];
    if (head == NULL) {
        adjacency[node->fromID] = node;
        return;
    }
    while (head->next != NULL) {
        head = head->next;
    }
    head->next = node;
}

void Graph::insertRoadNodes(const Road& road) {
    appendNode(createNode(road, false));
    if (road.isTwoWay) {
        appendNode(createNode(road, true));
    }
}

int Graph::addRoad(int fromID, int toID, double distance, double travelTime, bool twoWay) {
    Road road(nextRoadID, fromID, toID, distance, travelTime, true, twoWay);
    if (!checkNewRoad(road)) {
        return -1;
    }
    insertRoadNodes(road);
    int newID = nextRoadID;
    nextRoadID++;
    return newID;
}

bool Graph::getRoad(int roadID, Road& out) const {
    int total = adjacency.size();
    for (int i = 0; i < total; i++) {
        for (const RoadNode* node = adjacency[i]; node != NULL; node = node->next) {
            if (node->roadID != roadID) {
                continue;
            }
            if (!node->isTwoWay || node->fromID < node->toID) {
                out = nodeToRoad(*node);
                return true;
            }
        }
    }
    return false;
}

const RoadNode* Graph::getOutgoing(int id) const {
    if (!isValidLocation(id)) {
        return NULL;
    }
    return adjacency[id];
}

int Graph::roadCount() const {
    int count = 0;
    int total = adjacency.size();
    for (int i = 0; i < total; i++) {
        for (const RoadNode* node = adjacency[i]; node != NULL; node = node->next) {
            if (!node->isTwoWay || node->fromID < node->toID) {
                count++;
            }
        }
    }
    return count;
}

void Graph::printLocations(ostream& os) const {
    os << "Locations (" << activeCount << "):\n";
    if (activeCount == 0) {
        os << "  (none)\n";
        return;
    }
    int total = locations.size();
    for (int i = 0; i < total; i++) {
        if (locations[i].active) {
            os << "  [" << i << "] " << locations[i].name << "\n";
        }
    }
}

void Graph::printMap(ostream& os) const {
    printLocations(os);
    os << "Roads (" << roadCount() << "):\n";

    bool any = false;
    int total = adjacency.size();
    for (int i = 0; i < total; i++) {
        for (const RoadNode* node = adjacency[i]; node != NULL; node = node->next) {
            if (node->isTwoWay && node->fromID > node->toID) {
                continue;
            }
            any = true;

            ostringstream line;
            line << fixed << setprecision(2);
            line << "  #" << node->roadID << "  " << nameOf(node->fromID);
            if (node->isTwoWay) {
                line << " <-> ";
            } else {
                line << " -> ";
            }
            line << nameOf(node->toID) << "  |  " << node->distance << " km  |  "
                 << node->travelTime << " min  |  ";
            if (node->isOpen) {
                line << "OPEN";
            } else {
                line << "CLOSED";
            }
            os << line.str() << "\n";
        }
    }
    if (!any) {
        os << "  (none)\n";
    }
}
