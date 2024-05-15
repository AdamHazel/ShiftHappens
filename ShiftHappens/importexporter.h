#ifndef IMPORTEXPORTER_H
#define IMPORTEXPORTER_H

#include "shiftdatabase.h"
#include "car.h"
#include "customer.h"

#include <QObject>
#include <vector>

class importExporter
{
protected:
    std::vector<car> allCars;
    std::vector<customer> allCustomers;

public:
    void importCars(shiftDatabase &dataB);
    void importCustomers(shiftDatabase &dataB);
    void create_templates();

    void exportCustomers(shiftDatabase &dataB);
    void exportCars(shiftDatabase &dataB);
    void exportAll(shiftDatabase &dataB);

};

#endif // IMPORTEXPORTER_H
