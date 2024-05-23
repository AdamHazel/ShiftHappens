/**
 * @class Manages basic transactions with database
 */

#ifndef SHIFTDATABASE_H
#define SHIFTDATABASE_H

#include "rental.h"
#include "car.h"
#include "customer.h"

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
    void rental_createTable();

    // Car functions
    bool cars_add(QString *RegNr, QString *brand, QString *model, uint *dayPrice);
    void cars_fetchById(QString& regNr, QString& brand, QString& model, uint& dayPrice);
    car cars_fetchCar(QString& regNr);
    bool cars_updateByRegNr(QString *RegNr, QString *brand, QString *model, uint *dayPrice);
    bool cars_removeByRegNr(QString *RegNr);
    uint cars_countEntries();
    std::vector<car> cars_fetchAll();
    void cars_dropTable();
    void cars_resetTable();
    bool cars_checkExistence(QString &RegNr);
    uint cars_fetchDayPrice(QString &RegNr);

    // Customer functions
    bool customer_add(QString *name, QString *street, uint *postNum, QString *city);
    void customer_fetchById(uint& id, QString& name, QString& street, uint& postNum, QString& city);
    customer customer_fetchCustomer(uint& id);
    bool customer_updateByID(uint *idChoice, QString *name, QString *street, uint *postNum, QString *city);
    bool customer_removeByID(uint *idChoice);
    uint customer_countEntries();
    std::vector<customer> customer_fetchAll();
    void customer_dropTable();
    void customer_resetTable();
    bool customer_checkExistence(uint &id);

    // Rental functions
    bool rental_addRental(QString& RegNr, uint& custId, QString& startDate, QString& endDate, uint& totalPrice);
    int rental_countRecords();
    std::vector<rental> rental_FetchAll();
    bool rental_removeRental(uint &id);
    bool rental_removeRentalonCustomer(uint &custId);
    bool rental_removeRentalonCar(QString& RegNr);
    bool rental_completeRental(uint &id);
    void rental_dropTable();
    void rental_resetTable();


    // Extra functions
    void databaseOpen();
    void databaseClose();

};

#endif // SHIFTDATABASE_H
