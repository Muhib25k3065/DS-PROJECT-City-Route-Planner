#ifndef CHANGERECORD_H
#define CHANGERECORD_H

#include "Road.h"

enum ChangeType {
    CHANGE_ADD_ROAD,
    CHANGE_REMOVE_ROAD,
    CHANGE_UPDATE_ROAD
};

struct ChangeRecord {
    ChangeType type;
    Road oldRoad;

    ChangeRecord() {
        type = CHANGE_ADD_ROAD;
    }

    ChangeRecord(ChangeType changeType, const Road& snapshot) {
        type = changeType;
        oldRoad = snapshot;
    }
};

#endif
