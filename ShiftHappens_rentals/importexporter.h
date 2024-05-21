#ifndef IMPORTEXPORTER_H
#define IMPORTEXPORTER_H

#include "shiftdatabase.h"
#include "car.h"
#include "customer.h"
#include "rental.h"

#include <QObject>
#include <vector>

class importExporter
{
protected:
    std::vector<car> allCars;
    std::vector<customer> allCustomers;
    std::vector<rental> allRentals;

public:
    void importCars(shiftDatabase &dataB);
    void importCustomers(shiftDatabase &dataB);
    void importRentals(shiftDatabase &dataB);
    void create_templates();

    void exportCustomers(shiftDatabase &dataB);
    void exportCars(shiftDatabase &dataB);
    void exportRentals(shiftDatabase &dataB);
    void exportAll(shiftDatabase &dataB);
};

#endif // IMPORTEXPORTER_H
