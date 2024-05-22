/**
 * @class A representation of a car with corresponding values that are saved in database
 */

#ifndef CAR_H
#define CAR_H

#include <QObject>

class car
{

public:
    QString RegNr;
    QString brand;
    QString model;
    uint dayPrice {};

};
#endif // CAR_H
