#ifndef RENTAL_H
#define RENTAL_H

#include <QObject>
#include <QDate>

class rental
{
public:
    uint rentalId;
    QString RegNr;
    uint custId;
    QDate startDate;
    QDate endDate;
    uint totalPrice;
    bool completed;
};

#endif // RENTAL_H
