#include "shiftdatabase.h"

shiftDatabase::shiftDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    qDebug() << QDir::currentPath();
    db.setDatabaseName(QDir::currentPath() + "/shiftDatabase.db");

    if(db.open())
    {
        qDebug() << "Database is open";
        customer_createTable();
        cars_createTable();
        //test();
        db.close();
    }

    else
        qDebug() << "Database could not open: " << db.lastError();
}

// Creation of tables
void shiftDatabase::customer_createTable()
{
    QSqlQuery createCustomers(db);
    createCustomers.prepare(" CREATE TABLE IF NOT EXISTS customers ("
                            " id INTEGER PRIMARY KEY AUTOINCREMENT,"
                            " name TEXT,"
                            " street TEXT,"
                            " postcode INT NOT NULL,"
                            " city TEXT)");

    if (createCustomers.exec())
        qDebug() << "Customer table created or exists already";
    else
        qDebug() << "Failed to add table" << db.lastError();
}

void shiftDatabase::cars_createTable()
{
    QSqlQuery createCars(db);
    createCars.prepare("CREATE TABLE IF NOT EXISTS cars ("
                       "RegNr TEXT PRIMARY KEY,"
                       "brand TEXT, "
                       "model TEXT, "
                       "year NUMERIC NOT NULL, "
                       "custId INTEGER DEFAULT NULL, "
                       "FOREIGN KEY (custId) REFERENCES customer(id))");

    if (createCars.exec())
        qDebug() << "Cars table created or exists already";
    else
        qDebug() << "Failed to add table" << db.lastError();
}

// Finders
bool shiftDatabase::customer_findById(int *idChoice)
{
    bool exists = false;
    db.open();

    qDebug() << "Query to start finding customer starts";
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) "
                  "FROM customers "
                  "WHERE id = ?");
    query.addBindValue((*idChoice));

    if (query.exec())
    {
        qDebug() << "Query executed";
        int count {};
        while (query.next())
        {
            count = query.value(0).toInt();
        }

        switch (count)
        {
        case 0:
            qDebug() << "Car not found: " << count;
            break;
        case 1:
            qDebug() << "Car found";
            exists = true;
            break;
        default:
            qDebug() << "Error as result of count was not 0 or 1";
            break;
        }
    }
    else
    {
        qDebug() << "Query to find customer by id not executed" << db.lastError();
    }
    db.close();
    return exists;
}

bool shiftDatabase::cars_findByRegNr(QString *regChoice)
{
    bool exists = false;
    db.open();

    qDebug() << "Query to start finding car starts";
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) "
                  "FROM cars "
                  "WHERE RegNr LIKE ?");
    query.addBindValue((*regChoice));

    if (query.exec())
    {
        qDebug() << "Query executed";
        int count {};
        while (query.next())
        {
            count = query.value(0).toInt();
        }

        switch (count)
        {
        case 0:
            qDebug() << "Car not found";
            break;
        case 1:
            qDebug() << "Car found";
            exists = true;
            break;
        default:
            qDebug() << "Error as result of count was not 0 or 1";
            break;
        }
    }
    else
    {
        qDebug() << "Query to find car by regNr not executed" << db.lastError();;
    }

    db.close();
    return exists;
}


// Counters
int shiftDatabase::customer_countEntries()
{
    int count {};
    db.open();
    qDebug() << "Query to start counting how many customers exists";
    QSqlQuery query;
    query.prepare("SELECT COUNT (*) "
                  "FROM customers");
    if (query.exec())
    {
        qDebug() << "Query executed";
        while (query.next())
            count = query.value(0).toInt();
        if (count == 0)
            qDebug() << "No customers in table";
        if (count > 0)
            qDebug() << "Customers in table is " << count;
        else
            qDebug() << "Query to find amount of customers did not work" << db.lastError();
    }
    db.close();
    return count;
}


int shiftDatabase::cars_countEntries()
{
    int count {};
    db.open();
    qDebug() << "Query to start counting how many cars exists";
    QSqlQuery query;
    query.prepare("SELECT COUNT (*) "
                  "FROM cars");
    if (query.exec())
    {
        qDebug() << "Query executed";
        while (query.next())
            count = query.value(0).toInt();

        if (count == 0)
            qDebug() << "No cars in table";

        if (count > 0)
            qDebug() << "Cars in table is " << count;
    }
    else
        qDebug() << "Query to find amount of customers did not work" << db.lastError();

    db.close();

    return count;
}

// Fetchers
void shiftDatabase::cars_fetchById(QString& regNr, QString& brand, QString& model, uint& year){
    db.open();
    QSqlQuery query;
    query.prepare("SELECT Brand, Model, Year FROM cars WHERE RegNr = :RegNr");
    query.bindValue(":RegNr", regNr);

    if (query.exec() && query.next()) {
        qDebug() << "Car found";
        brand = query.value(0).toString();
        model = query.value(1).toString();
        year = query.value(2).toUInt();
        db.close();
        }
    else {
        qDebug() << "Car not found or query failed: " << db.lastError();
        db.close();
        }
}
void shiftDatabase::customer_fetchById(uint& id, QString& name, QString& street, uint& postNum, QString& city)
{
    db.open();
    QSqlQuery query;
    query.prepare("SELECT name, street, postcode, city FROM customers WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        qDebug() << "Customer found";
        name = query.value(0).toString();
        street = query.value(1).toString();
        postNum = query.value(2).toUInt();
        city = query.value(3).toString();
        db.close();
    }
    else {
        qDebug() << "Customer not found or query failed: " << db.lastError();
        db.close();
    }
}

// Adders
bool shiftDatabase::customer_add(QString *name, QString *street, uint *postNum, QString *city)
{
    bool executed = false;
    db.open();
    QSqlDatabase::database().transaction();

    qDebug() << "Query begun";

    QSqlQuery addCustQuery(db);
    addCustQuery.prepare("INSERT into customers "
                         "(name, street, postcode, city) "
                         "VALUES "
                         "(?, ?, ?, ?)");
    addCustQuery.addBindValue((*name));
    addCustQuery.addBindValue((*street));
    addCustQuery.addBindValue((*postNum));
    addCustQuery.addBindValue((*city));

    if(addCustQuery.exec())
    {
        qDebug() << "Query executed";
        executed = true;
    }
    else
    {
        qDebug() << "Query not executed" << db.lastError();
    }

    QSqlDatabase::database().commit();
    db.close();

    return executed;
}

bool shiftDatabase::cars_add(QString *RegNr, QString *brand, QString *model, uint *year)
{
    bool executed = false;
    db.open();
    QSqlDatabase::database().transaction();

    qDebug() << "Query begun";

    QSqlQuery addCarQuery(db);
    addCarQuery.prepare("INSERT into cars "
                        "(RegNr, brand, model, year) "
                        "VALUES "
                        "(?, ?, ?, ?)");
    addCarQuery.addBindValue((*RegNr).toUpper());
    addCarQuery.addBindValue((*brand));
    addCarQuery.addBindValue((*model));
    addCarQuery.addBindValue((*year));

    if(addCarQuery.exec())
    {
        qDebug() << "Query executed";
        qDebug() << (*RegNr);
        executed = true;
    }
    else
    {
        qDebug() << "Query not executed" << db.lastError();
    }

    QSqlDatabase::database().commit();
    db.close();

    return executed;
}

// Updaters
bool shiftDatabase::customer_updateByID(uint *idChoice, QString *name, QString *street, uint *postNum, QString *city)
{
    bool confirm = false;
    db.open();

    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("UPDATE customers "
                  "SET name = ?, "
                  "street = ?,"
                  "postcode = ?,"
                  "city = ? "
                  "WHERE id = ?");

    query.bindValue(0, (*name));
    query.bindValue(1, (*street));
    query.bindValue(2, (*postNum));
    query.bindValue(3, (*city));
    query.bindValue(4, (*idChoice));

    if(query.exec())
    {
        if(query.numRowsAffected() == 1)
        {
            qDebug() << "One row affected";
            confirm = true;
        }
        else
            qDebug() << "Something went wrong as number of rows affected was " << query.numRowsAffected();
    }
    else
        qDebug() << "Query to update row failed to execute";

    QSqlDatabase::database().commit();
    db.close();
    return confirm;
}

bool shiftDatabase::cars_updateByRegNr(QString *RegNr, QString *brand, QString *model, uint *year)
{
    bool confirm = false;
    db.open();

    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("UPDATE cars "
                  "SET model = ?, "
                  "brand = ?,"
                  "year = ? "
                  "WHERE RegNr = ?");

    query.bindValue(0, (*model));
    query.bindValue(1, (*brand));
    query.bindValue(2, (*year));
    query.bindValue(3, (*RegNr).toUpper());

    if(query.exec())
    {
        if(query.numRowsAffected() == 1)
        {
            qDebug() << "One row affected";
            confirm = true;
        }
        else
            qDebug() << "Something went wrong as number of rows affected was " << query.numRowsAffected();
    }
    else
        qDebug() << "Query to update row failed to execute" << db.lastError();

    QSqlDatabase::database().commit();
    db.close();
    return confirm;
}
bool shiftDatabase::cars_removeByRegNr(QString *RegNr){
    bool executed = false;
    db.open();

    QSqlDatabase::database().transaction();
    QSqlQuery query;
    query.prepare("DELETE FROM cars WHERE RegNr = :RegNr");
    query.bindValue(":RegNr", *RegNr);
    if(query.exec())
    {
        qDebug() << "Query executed";
        qDebug() << (*RegNr);
        executed = true;
    }
    else
        qDebug() << "Query not executed" << db.lastError();

    QSqlDatabase::database().commit();
    db.close();
    return executed;
}
bool shiftDatabase::customer_removeByID(uint *idChoice){
    bool executed = false;
    db.open();

    QSqlDatabase::database().transaction();
    QSqlQuery query;

    query.prepare("DELETE FROM customers WHERE id = :id");
    query.bindValue(":id", *idChoice);
    if(query.exec())
    {
        qDebug() << "Query executed";
        qDebug() << (*idChoice);
        executed = true;
    }
    else
        qDebug() << "Query not executed" << db.lastError();

    QSqlDatabase::database().commit();
    db.close();

    return executed;
}

// For testing that the table works
void shiftDatabase::test()
{
    db.open();
    QSqlDatabase::database().transaction();

    qDebug() << "Query begun";

    QString RegNr = "TR 1234";
    RegNr = RegNr.toUpper();
    QString Model = "Kathe";
    QString Brand = "BMW";
    double dayPrice = 123.50;

    QSqlQuery addCarQuery(db);
    addCarQuery.prepare("INSERT into cars"
                        "(RegNr, model, brand, dayPrice)"
                        "VALUES"
                        "(?, ?, ?, ?)");

    addCarQuery.addBindValue(RegNr);
    addCarQuery.addBindValue(Model);
    addCarQuery.addBindValue(Brand);
    addCarQuery.addBindValue(dayPrice);

    if(addCarQuery.exec())
    {
        qDebug() << "Query executed";
    }
    else
    {
        qDebug() << "Query not executed" << db.lastError();
    }

    QSqlDatabase::database().commit();
    db.close();
}
