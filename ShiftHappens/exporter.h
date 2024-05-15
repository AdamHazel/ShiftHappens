#ifndef EXPORTER_H
#define EXPORTER_H

#include "shiftdatabase.h"
#include "car.h"
#include "customer.h"

#include <QObject>
#include <vector>

class exporter
{    
protected:
    std::vector<car> allCars;
    std::vector<customer> allCustomers;

public:
    void exportCustomers(shiftDatabase &dataB);
    void exportCars(shiftDatabase &dataB);
    void exportAll(shiftDatabase &dataB);
};

#endif // EXPORTER_H
