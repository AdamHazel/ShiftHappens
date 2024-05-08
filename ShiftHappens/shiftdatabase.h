#ifndef SHIFTDATABASE_H
#define SHIFTDATABASE_H

#include "car.h"
#include "customer.h"

#include <QtSql>
#include <vector>

class shiftDatabase : public QObject
{
    Q_OBJECT

protected:
    QSqlDatabase db;

public:
    shiftDatabase();

    // Table creators
    void customer_createTable();
    void cars_createTable();

    // Finders
    bool customer_findById(int *idChoice);
    bool cars_findByRegNr(QString *regChoice);

    // Counters
    int customer_countEntries();
    int cars_countEntries();

    // Fetchers
    std::vector<customer> customer_fetchAll();
    std::vector<car> cars_fetchAll();
    void customer_fetchById(uint& id, QString& name, QString& street, uint& postNum, QString& city);
    void cars_fetchById(QString& regNr, QString& brand, QString& model, uint& year);

    // Adders
    bool customer_add(QString *name, QString *street, uint *postNum, QString *city);
    bool cars_add(QString *RegNr, QString *brand, QString *model, uint *year);

    // Updaters
    bool customer_updateByID(uint *idChoice, QString *name, QString *street, uint *postNum, QString *city);
    bool cars_updateByRegNr(QString *RegNr, QString *brand, QString *model, uint *year);

    // Removers
    bool customer_removeByID(uint *idChoice);
    bool cars_removeByRegNr(QString *RegNr);

    // Droppers
    void customer_dropTable();
    void cars_dropTable();

    // Resetters
    void customer_resetTable();
    void cars_resetTable();
};

#endif // SHIFTDATABASE_H
