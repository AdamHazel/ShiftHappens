/**
 * @class A representation of a customer with corresponding values that are saved in database
 */

#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <QObject>

class customer
{

public:
    uint id;
    QString name;
    QString street;
    uint postcode;
    QString city;

};
#endif // CUSTOMER_H
