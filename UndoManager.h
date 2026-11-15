#ifndef UNDOMANAGER_H
#define UNDOMANAGER_H

#include <string>
#include "ChangeRecord.h"
#include "Graph.h"
#include "LinkedStack.h"
using namespace std;

class UndoManager {
private:
    LinkedStack<ChangeRecord> history;

public:
    UndoManager();

    int addRoad(Graph& graph, int fromID, int toID, double distance, double travelTime, bool twoWay);
    bool removeRoad(Graph& graph, int roadID);
    bool updateRoad(Graph& graph, int roadID, double newDistance, double newTravelTime);
    bool setRoadOpen(Graph& graph, int roadID, bool open);

    bool undoLast(Graph& graph, string& message);

    bool canUndo() const;
    int historySize() const;
    string describeNextUndo() const;
    void clear();
};

#endif
