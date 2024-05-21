#ifndef RENTALMANAGER_H
#define RENTALMANAGER_H

#include "rental.h"
#include "shiftdatabase.h"

#include <QObject>
#include <vector>

class rentalManager
{
protected:
    std::vector<rental> rentals;

public:
    bool checkRental(shiftDatabase &dataB, rental &checking);
};

#endif // RENTALMANAGER_H
