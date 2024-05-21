#include "shiftdatabase.h"
#include "rental.h"

#include <vector>
#include <QMessageBox>

shiftDatabase::shiftDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    qDebug() << QDir::currentPath();
    db.setDatabaseName(QDir::currentPath() + "/shiftDatabase.db");
}


// Functions that manage car table
/**
 * @brief Creates car table in database
*/
void shiftDatabase::cars_createTable()
{
    db.open();
    QSqlQuery createCars(db);
    createCars.prepare("CREATE TABLE IF NOT EXISTS cars ("
                       "RegNr TEXT PRIMARY KEY,"
                       "brand TEXT, "
                       "model TEXT, "
                       "dayPrice NUMERIC NOT NULL)");

    if (createCars.exec())
        qDebug() << "Cars table created or exists already";
    else
        qDebug() << "Failed to add cars table" << db.lastError();
    db.close();
}

/**
 * @brief Adds record to car table in database
 * @param QString*      pointer to variable that holds regNr
 * @param QString*      pointer to variable that holds brand
 * @param QString*      pointer to variable that holds model
 * @param uint*         pointer to variable that holds dayprice
 * @returns confirmation if query was successful
*/
bool shiftDatabase::cars_add(QString *RegNr, QString *brand, QString *model, uint *dayPrice)
{
    bool executed = false;
    db.open();

    qDebug() << "Query begun";

    QSqlQuery addCarQuery(db);
    addCarQuery.prepare("INSERT into cars "
                        "(RegNr, brand, model, dayPrice) "
                        "VALUES "
                        "(?, ?, ?, ?)");
    addCarQuery.addBindValue((*RegNr).toUpper());
    addCarQuery.addBindValue((*brand));
    addCarQuery.addBindValue((*model));
    addCarQuery.addBindValue((*dayPrice));

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
    db.close();

    return executed;
}

/**
 * @brief Fetches record from car table in database
 * @param QString&      reference to variable that will hold regNr
 * @param QString&      reference to variable that will hold brand
 * @param QString&      reference to variable that will hold model
 * @param uint&         reference to variable that will hold dayprice
*/
void shiftDatabase::cars_fetchById(QString& regNr, QString& brand, QString& model, uint& dayPrice){
    db.open();
    QSqlQuery query;
    query.prepare("SELECT brand, model, dayPrice FROM cars WHERE RegNr = :RegNr");
    query.bindValue(":RegNr", regNr);

    if (query.exec() && query.next()) {
        qDebug() << "Car found";
        brand = query.value(0).toString();
        model = query.value(1).toString();
        dayPrice = query.value(2).toUInt();
    }
    else {
        qDebug() << "Car not found or query failed: " << db.lastError();
    }
    db.close();
}

/**
 * @brief Updates record of car table in database
 * @param QString*      pointer to variable that holds regNr
 * @param QString*      pointer to variable that holds brand
 * @param QString*      pointer to variable that holds model
 * @param uint*         pointer to variable that holds dayprice
 * @returns confirmation if query was successful
*/
bool shiftDatabase::cars_updateByRegNr(QString *RegNr, QString *brand, QString *model, uint *dayPrice)
{
    bool confirm = false;
    db.open();

    QSqlQuery query;
    query.prepare("UPDATE cars "
                  "SET model = ?, "
                  "brand = ?,"
                  "dayPrice = ? "
                  "WHERE RegNr = ?");

    query.bindValue(0, (*model));
    query.bindValue(1, (*brand));
    query.bindValue(2, (*dayPrice));
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

    db.close();
    return confirm;
}

/**
 * @brief Deletes record of car table in database
 * @param QString*      pointer to variable that holds regNr
 * @returns confirmation if query was successful
*/
bool shiftDatabase::cars_removeByRegNr(QString *RegNr){
    bool executed = false;
    db.open();

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

    db.close();
    return executed;
}

// Functions that manage customer table
/**
 * @brief Creates customer table in database
*/
void shiftDatabase::customer_createTable()
{
    db.open();
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
        qDebug() << "Failed to add customer table" << db.lastError();
    db.close();
}

/**
 * @brief Adds record to customer table in database
 * @param QString*      pointer to variable that holds name
 * @param QString*      pointer to variable that holds street
 * @param uint*         pointer to variable that holds postcode
 * @param QString*      pointer to variable that holds city
 * @returns bool value to show if query was successful
*/
bool shiftDatabase::customer_add(QString *name, QString *street, uint *postNum, QString *city)
{
    bool executed = false;
    db.open();

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

    db.close();

    return executed;
}

/**
 * @brief Fetches record from customers table in database
 * @param uint&         reference to variable that will hold id
 * @param QString&      reference to variable that will hold name
 * @param QString&      reference to variable that will hold street
 * @param uint&         reference to variable that will hold postcode
 * @param QString&      reference to variable that will hold city
*/
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
    }
    else {
        qDebug() << "Customer not found or query failed: " << db.lastError();
    }
    db.close();
}

/**
 * @brief Updates record of customer table in database
 * @param uint*         pointer to variable that holds id
 * @param QString*      pointer to variable that holds name
 * @param QString*      pointer to variable that holds street
 * @param uint*         pointer to variable that holds postcode
 * @param QString*      pointer to variable that holds city
 * @returns bool         shows if query was successful
*/
bool shiftDatabase::customer_updateByID(uint *idChoice, QString *name, QString *street, uint *postNum, QString *city)
{
    bool confirm = false;
    db.open();

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

    db.close();
    return confirm;
}

/**
 * @brief Deletes record of customer table in database
 * @param QString*      pointer to variable that holds customer ID
 * @returns confirmation if query was successful
*/
bool shiftDatabase::customer_removeByID(uint *idChoice){
    bool executed = false;
    db.open();

    QSqlQuery query;

    query.prepare("DELETE FROM customers WHERE id = :id");
    query.bindValue(":id", *idChoice);
    if(query.exec())
    {
        qDebug() << "Query executed";
        if (query.numRowsAffected() == 1)
        {
            qDebug() << (*idChoice);
            executed = true;
        }
    }
    else
        qDebug() << "Query not executed" << db.lastError();

    db.close();

    return executed;
}

// Functions that manage rentals table
/**
 * @brief Creates rentals table in database (foreign keys managed by program rather than database)
*/
void shiftDatabase::rentals_createTable()
{
    db.open();
    QSqlQuery createCars(db);
    createCars.prepare("CREATE TABLE IF NOT EXISTS rentals ("
                       "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                       "RegNr TEXT, "
                       "custId INTERGER, "
                       "startDate TEXT NOT NULL, "
                       "endDate TEXT NOT NULL, "
                       "totalPrice NUMERIC, "
                       "completed INTEGER NOT NULL CHECK (completed IN (0,1)))");

    if (createCars.exec())
        qDebug() << "Rentals table created or exists already";
    else
        qDebug() << "Failed to add rentals table" << db.lastError();
    db.close();
}

/**
 * @brief Adds record to rentals table in database
 * @param QString&      reference to variable that holds RegNr
 * @param uint&         reference to variable that holds customerId
 * @param QString&      reference to variable that holds startDate
 * @param QString&      reference to variable that holds endDate
 * @param uint&         reference to variable that holds totalPrice
 * @returns bool value to show if query was successful
*/
bool shiftDatabase::rental_addRental(QString& RegNr, uint& custId, QString& startDate, QString& endDate, uint& totalPrice)
{
    bool executed = false;
    db.open();

    qDebug() << "Query begun";

    QSqlQuery addrentalQuery(db);
    addrentalQuery.prepare("INSERT into rentals "
                         "(RegNr, custId, startDate, endDate, totalPrice, completed) "
                         "VALUES "
                         "(?, ?, ?, ?, ?, 0)");
    addrentalQuery.addBindValue((RegNr));
    addrentalQuery.addBindValue((custId));
    addrentalQuery.addBindValue((startDate));
    addrentalQuery.addBindValue((endDate));
    addrentalQuery.addBindValue((totalPrice));

    if(addrentalQuery.exec())
    {
        qDebug() << "Query executed";
        if(addrentalQuery.numRowsAffected() == 1)
        {
            executed = true;
        }
    }
    else
    {
        qDebug() << "Query not executed" << db.lastError();
    }

    db.close();

    return executed;
}

/**
 * @brief Counts how many records are in rental table
 * @returns Number of records in rental table
*/
int shiftDatabase::rental_countRecords()
{
    int count {};
    db.open();
    qDebug() << "Query to start counting how many cars exists";
    QSqlQuery query;
    query.prepare("SELECT COUNT (*) "
                  "FROM rentals");
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
    {
        count = -1;
        qDebug() << "Query to find amount of customers did not work" << db.lastError();
    }

    db.close();

    return count;
}

/**
 * @brief Returns all records that are in rental table
 * @returns Records in rental table as a
*/
std::vector<rental> shiftDatabase::rental_FetchAll()
{
    std::vector<rental> rentals;
    QSqlQuery fetchAll(db);
    fetchAll.prepare("SELECT * FROM rentals");
    fetchAll.exec();
    QSqlRecord rec = fetchAll.record();

    int idIndex = rec.indexOf("id");
    int regnrIndex = rec.indexOf("RegNr");
    int custidIndex = rec.indexOf("custId");
    int sdateIndex = rec.indexOf("startDate");
    int edateIndex = rec.indexOf("endDate");
    int totalpriceIndex = rec.indexOf("totalPrice");
    int completedIndex = rec.indexOf("completed");

    while(fetchAll.next())
    {
        rental temp;
        temp.rentalId = fetchAll.value(idIndex).toInt();
        temp.RegNr = fetchAll.value(regnrIndex).toString();
        temp.custId = fetchAll.value(custidIndex).toInt();
        temp.startDate = fetchAll.value(sdateIndex).toDate();
        temp.endDate = fetchAll.value(edateIndex).toDate();
        temp.totalPrice = fetchAll.value(totalpriceIndex).toInt();
        temp.completed = fetchAll.value(completedIndex).toBool();
        rentals.push_back(temp);
    }

    qDebug() << "Finished fetching all rows in customers table";

    db.close();

    return rentals;
}

bool shiftDatabase::rental_removeRental(uint &id)
{
    bool executed = false;
    db.open();

    QSqlQuery query;

    query.prepare("DELETE FROM rentals WHERE id = ?");
    query.addBindValue(id);
    if(query.exec())
    {
        qDebug() << "Query executed";
        if (query.numRowsAffected() == 1)
        {
            executed = true;
        }
    }
    else
        qDebug() << "Query not executed" << db.lastError();

    db.close();

    return executed;
}

bool shiftDatabase::rental_completeRental(uint &id)
{
    bool executed = false;
    db.open();

    QSqlQuery query;

    query.prepare("UPDATE rentals SET completed = 1 WHERE id = ?");
    query.addBindValue(id);
    if(query.exec())
    {
        qDebug() << "Query executed";
        if (query.numRowsAffected() == 1)
        {
            executed = true;
        }
    }
    else
        qDebug() << "Query not executed" << db.lastError();

    db.close();

    return executed;
}
