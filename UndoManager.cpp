#include "UndoManager.h"
#include <sstream>
using namespace std;

UndoManager::UndoManager() {
}

int UndoManager::addRoad(Graph& graph, int fromID, int toID, double distance,
                         double travelTime, bool twoWay) {
    int roadID = graph.addRoad(fromID, toID, distance, travelTime, twoWay);
    if (roadID < 0) {
        return -1;
    }
    Road snapshot;
    if (graph.getRoad(roadID, snapshot)) {
        history.push(ChangeRecord(CHANGE_ADD_ROAD, snapshot));
    }
    return roadID;
}

bool UndoManager::removeRoad(Graph& graph, int roadID) {
    Road snapshot;
    bool existed = graph.getRoad(roadID, snapshot);
    if (!graph.removeRoad(roadID)) {
        return false;
    }
    if (existed) {
        history.push(ChangeRecord(CHANGE_REMOVE_ROAD, snapshot));
    }
    return true;
}

bool UndoManager::updateRoad(Graph& graph, int roadID, double newDistance, double newTravelTime) {
    Road snapshot;
    bool existed = graph.getRoad(roadID, snapshot);
    if (!graph.updateRoad(roadID, newDistance, newTravelTime)) {
        return false;
    }
    if (existed) {
        history.push(ChangeRecord(CHANGE_UPDATE_ROAD, snapshot));
    }
    return true;
}

bool UndoManager::setRoadOpen(Graph& graph, int roadID, bool open) {
    Road snapshot;
    bool existed = graph.getRoad(roadID, snapshot);
    if (!graph.setRoadOpen(roadID, open)) {
        return false;
    }
    if (existed) {
        history.push(ChangeRecord(CHANGE_UPDATE_ROAD, snapshot));
    }
    return true;
}

bool UndoManager::undoLast(Graph& graph, string& message) {
    ChangeRecord record;
    if (!history.pop(record)) {
        message = "Nothing to undo.";
        return false;
    }

    ostringstream text;
    bool ok = false;

    if (record.type == CHANGE_ADD_ROAD) {
        ok = graph.removeRoad(record.oldRoad.roadID);
        text << "Undid the addition of road #" << record.oldRoad.roadID << " (road removed).";
    } else if (record.type == CHANGE_REMOVE_ROAD) {
        ok = graph.restoreRoad(record.oldRoad);
        text << "Undid the removal of road #" << record.oldRoad.roadID << " (road restored).";
    } else {
        ok = graph.updateRoad(record.oldRoad.roadID, record.oldRoad.distance, record.oldRoad.travelTime);
        if (ok) {
            ok = graph.setRoadOpen(record.oldRoad.roadID, record.oldRoad.isOpen);
        }
        text << "Undid the update of road #" << record.oldRoad.roadID << " (old values restored).";
    }

    if (!ok) {
        message = "Undo failed: " + graph.getLastError();
        return false;
    }
    message = text.str();
    return true;
}

bool UndoManager::canUndo() const {
    return !history.isEmpty();
}

int UndoManager::historySize() const {
    return history.size();
}

string UndoManager::describeNextUndo() const {
    ChangeRecord record;
    if (!history.peek(record)) {
        return "Nothing to undo.";
    }

    ostringstream text;
    if (record.type == CHANGE_ADD_ROAD) {
        text << "Next undo: remove the road that was added (road #" << record.oldRoad.roadID << ").";
    } else if (record.type == CHANGE_REMOVE_ROAD) {
        text << "Next undo: bring back the removed road #" << record.oldRoad.roadID << ".";
    } else {
        text << "Next undo: restore the previous values of road #" << record.oldRoad.roadID << ".";
    }
    return text.str();
}

void UndoManager::clear() {
    history.clear();
}
