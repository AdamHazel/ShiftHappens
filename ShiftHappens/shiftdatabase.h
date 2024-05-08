#ifndef SHIFTDATABASE_H
#define SHIFTDATABASE_H

#include <QtSql>

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

    // Test
    void test();
};

#endif // SHIFTDATABASE_H
