#ifndef ROUTING_H
#define ROUTING_H

#include <ostream>
#include <vector>
#include "Graph.h"
#include "RouteResult.h"
using namespace std;

RouteResult findRoute(const Graph& graph, int startID, int endID, RouteMode mode);

RouteResult routeViaStop(const Graph& graph, int startID, int stopID, int endID, RouteMode mode);

vector<int> reconstructPath(const vector<int>& parent, int startID, int endID);

void printRoute(const Graph& graph, const RouteResult& result, ostream& os);

#endif
