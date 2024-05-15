#ifndef IMPORTER_H
#define IMPORTER_H

#include "shiftdatabase.h"
#include "car.h"
#include "customer.h"

#include <QObject>
#include <vector>


class importer
{   
protected:
    std::vector<car> allCars;
    std::vector<customer> allCustomers;

public:
    void importCars(shiftDatabase &dataB);
    void importCustomers(shiftDatabase &dataB);
    void create_templates();
};

#endif // IMPORTER_H
