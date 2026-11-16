#ifndef ROADMENU_H
#define ROADMENU_H

#include "Graph.h"
#include "UndoManager.h"

void runRoadMenu(Graph& graph, UndoManager& undo);

void runUndoAction(Graph& graph, UndoManager& undo);

#endif
