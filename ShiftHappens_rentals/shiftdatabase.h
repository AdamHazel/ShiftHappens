/**
 * @class Manages basic transactions with database
 */

#ifndef SHIFTDATABASE_H
#define SHIFTDATABASE_H

#include "rental.h"

#include <QObject>
#include <QtSql>
#include <vector>
class shiftDatabase
{

protected:
    QSqlDatabase db;
public:
    shiftDatabase();

    // Table creators
    void customer_createTable();
    void cars_createTable();
    void rentals_createTable();

    // Car functions
    bool cars_add(QString *RegNr, QString *brand, QString *model, uint *dayPrice);
    void cars_fetchById(QString& regNr, QString& brand, QString& model, uint& dayPrice);
    bool cars_updateByRegNr(QString *RegNr, QString *brand, QString *model, uint *dayPrice);
    bool cars_removeByRegNr(QString *RegNr);

    // Customer functions
    bool customer_add(QString *name, QString *street, uint *postNum, QString *city);
    void customer_fetchById(uint& id, QString& name, QString& street, uint& postNum, QString& city);
    bool customer_updateByID(uint *idChoice, QString *name, QString *street, uint *postNum, QString *city);
    bool customer_removeByID(uint *idChoice);

    // Rental functions
    bool rental_addRental(QString& RegNr, uint& custId, QString& startDate, QString& endDate, uint& totalPrice);
    int rental_countRecords();
    std::vector<rental> rental_FetchAll();
    bool rental_removeRental(uint &id);
    bool rental_completeRental(uint &id);
    bool rental_uncompleteRental(uint &id);

};

#endif // SHIFTDATABASE_H
