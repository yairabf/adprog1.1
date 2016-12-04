//
// Created by hanani on 02/12/16.
//

#ifndef ADPROG1_1_TRIPINFO_H
#define ADPROG1_1_TRIPINFO_H


#include <stack>
#include "Point.h"
#include "Node.h"

class TripInfo {
private:
    int id;
    int metersPassed;
    Point start;
    Point end;
    int numberOfPassengers;
    float tariff;
    std::stack <Node*> *route = NULL;
public:
    int getId() const;

    int getMetersPassed() const;

    const Point &getStart() const;

    const Point &getEnd() const;

    int getNumberOfPassengers() const;

    float getTarrif() const;

    std::stack<Node *>* getRoute() const;
};


#endif //ADPROG1_1_TRIPINFO_H
