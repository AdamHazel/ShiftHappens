#include "importexporter.h"
#include "rapidcsv/rapidcsv.h"

#include <QMessageBox>
#include <fstream>

/**
*@brief Creates templates need to import data
*/
void importExporter::create_templates()
{
    // Customers template
    QString customersdesktopPath = QString("%1/customers_import.csv")
                                       .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    QByteArray custDP = customersdesktopPath.toLocal8Bit();
    const char* custPath = custDP.data();

    bool custTempAlreadyExists = false;

    if (std::filesystem::exists(custPath))
        custTempAlreadyExists = true;
    else
    {
        std::ofstream customers;
        customers.open(custPath);
        customers << "name,street,postcode,city\n";
        customers.close();
    }

    // Cars template
    QString carsdesktopPath = QString("%1/cars_import.csv")
                                  .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    QByteArray carsDP = carsdesktopPath.toLocal8Bit();
    const char* carsPath = carsDP.data();

    bool carTempAlreadyExists = false;

    if (std::filesystem::exists(carsPath))
        carTempAlreadyExists = true;
    else
    {
        std::ofstream cars;
        cars.open(carsPath);
        cars << "RegNr,brand,model,dayPrice\n";
        cars.close();
    }

    // Rentals template
    QString rentalsdesktopPath = QString("%1/rentals_import.csv")
                                  .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    QByteArray rentalsDP = rentalsdesktopPath.toLocal8Bit();
    const char* rentalsPath = rentalsDP.data();

    bool rentalTempAlreadyExists = false;

    if (std::filesystem::exists(rentalsPath))
        rentalTempAlreadyExists = true;
    else
    {
        std::ofstream rentals;
        rentals.open(rentalsPath);
        rentals << "RegNr,custId,startDate,endDate,totalPrice,completed\n";
        rentals.close();
    }

    QMessageBox confirmMsg;
    if (custTempAlreadyExists == false && carTempAlreadyExists == false && rentalTempAlreadyExists == false)
    {
        confirmMsg.setWindowTitle("Templates created");
        confirmMsg.setText("Templates saved to desktop.");
        confirmMsg.setDetailedText("Templates can be used to import cars and customers.\n"
                                   "File type is .csv. Use commas (,) to separate values.\n"
                                   "To import files, save files to desktop.\n"
                                   "Each entry should have a new row.\n"
                                   "Incase non-number values with \"\" .\n");
    } else
    {
        confirmMsg.setWindowTitle("Confirmation");
        confirmMsg.setText("Some or all templates existed already. Templates that did not exist were created.");
        confirmMsg.setDetailedText("Templates can be used to import cars and customers.\n"
                                   "File type is .csv. Use commas (,) to separate values.\n"
                                   "To import files, save files to desktop.\n"
                                   "Each entry should have a new row.\n"
                                   "Incase non-number values with \"\" .\n");
    }
    confirmMsg.exec();
}

/**
 * @brief Exports all customers to file saved on desktop
 * @param shiftDatabase&        reference to shiftDatabase object
 */
void importExporter::exportCustomers(shiftDatabase &dataB)
{
    uint customerExportCount { 0 };
    auto customerEntriesCount { dataB.customer_countEntries() };
    if (customerEntriesCount == 0)
    {
        QMessageBox noFileMsg;
        noFileMsg.setWindowTitle("Export confirmation");
        noFileMsg.setText("Customers table is empty. No file was generated.");
        noFileMsg.exec();
        return;
    }
    else
    {
        allCustomers = dataB.customer_fetchAll();
        QString customersdesktopPath = QString("%1/customers.csv")
                                           .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        QByteArray custDP = customersdesktopPath.toLocal8Bit();
        const char* custPath = custDP.data();

        std::ofstream customersFile;
        customersFile.open(custPath);
        customersFile << "id,name,street,postcode,city\n";

        for (auto &cust : allCustomers)
        {
            if (customersFile.is_open())
            {
                customersFile << cust.id << ",\"" << cust.name.toStdString() << "\",\"" << cust.street.toStdString() << "\","
                              << cust.postcode << ",\"" << cust.city.toStdString() << "\"\n";
                ++customerExportCount;
            }

        }
        qDebug() << "Amount of rows added to customer.csv: " << customerExportCount;
        customersFile.close();
        allCustomers.clear();
    }

    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle("Export customer confirmation");
    confirmMsg.setText(QString::number(customerExportCount) + " of " + QString::number(customerEntriesCount) + " customers exported.");
    if (customerExportCount != customerEntriesCount)
    {
        confirmMsg.setDetailedText("There was an error in exporting. Please contact developer.");
    }
    confirmMsg.exec();
}

/**
 * @brief Exports all cars to file saved on desktop
 * @param shiftDatabase&        reference to shiftDatabase object
 */
void importExporter::exportCars(shiftDatabase &dataB)
{
    uint carExportCount { 0 };
    auto carEntriesCount { dataB.cars_countEntries() };
    if (carEntriesCount == 0)
    {
        QMessageBox noFileMsg;
        noFileMsg.setWindowTitle("Export confirmation");
        noFileMsg.setText("Cars table is empty. No file was generated.");
        noFileMsg.exec();
        return;
    }
    else
    {
        allCars = dataB.cars_fetchAll();
        QString carsdesktopPath = QString("%1/cars.csv")
                                      .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        QByteArray carsDP = carsdesktopPath.toLocal8Bit();
        const char* carsPath = carsDP.data();

        std::ofstream carsFile;
        carsFile.open(carsPath);
        carsFile << "RegNr,brand,model,dayPrice\n";

        for (auto &car : allCars)
        {
            if (carsFile.is_open())
            {
                carsFile << "\"" << car.RegNr.toStdString() << "\",\"" << car.brand.toStdString() << "\",\"" << car.model.toStdString()
                         << "\"," << car.dayPrice << "\n";
                ++carExportCount;
            }

        }
        qDebug() << "Amount of rows added to cars.csv: " << carExportCount;
        carsFile.close();
        allCars.clear();
    }

    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle("Export cars confirmation");
    confirmMsg.setText(QString::number(carExportCount) + " of " + QString::number(carEntriesCount) + " cars exported.");
    if (carExportCount != carEntriesCount)
    {
        confirmMsg.setDetailedText("There was an error in exporting. Please contact developer.");
    }
    confirmMsg.exec();
}

/**
 * @brief Exports all rentals to file saved on desktop
 * @param shiftDatabase&        reference to shiftDatabase object
 */
void importExporter::exportRentals(shiftDatabase &dataB)
{
    int rentalExportCount { 0 };
    auto rentalEntriesCount { dataB.rental_countRecords() };
    qDebug() << "Amount of rental records is " << rentalEntriesCount;
    if (rentalEntriesCount == 0)
    {
        QMessageBox noFileMsg;
        noFileMsg.setWindowTitle("Export confirmation");
        noFileMsg.setText("Rentals table is empty. No file was generated.");
        noFileMsg.exec();
        return;
    }
    else
    {
        allRentals = dataB.rental_FetchAll();
        QString rentalsdesktopPath = QString("%1/rentals.csv")
                                      .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        QByteArray rentalsDP = rentalsdesktopPath.toLocal8Bit();
        const char* rentalsPath = rentalsDP.data();

        std::ofstream rentalsFile;
        rentalsFile.open(rentalsPath);
        rentalsFile << "id,RegNr,custId,startDate,endDate,totalPrice,completed\n";

        for (auto &rental : allRentals)
        {
            if (rentalsFile.is_open())
            {
                QString startD = rental.startDate.toString("dd-MM-yyyy");
                qDebug() << "start date is " << startD;
                QString endD = rental.endDate.toString("dd-MM-yyyy");
                rentalsFile  << rental.rentalId << ",\"" << rental.RegNr.toStdString() << "\"," << rental.custId
                            << ",\"" << startD.toStdString() << "\",\""
                            << endD.toStdString() << "\"," << rental.totalPrice << ","
                            << rental.completed << "\n";
                ++rentalExportCount;
            }

        }
        qDebug() << "Amount of rows added to cars.csv: " << rentalExportCount;
        rentalsFile.close();
        allRentals.clear();
    }

    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle("Export cars confirmation");
    confirmMsg.setText(QString::number(rentalExportCount) + " of " + QString::number(rentalEntriesCount) + " rentals exported.");
    if (rentalExportCount != rentalEntriesCount)
    {
        confirmMsg.setDetailedText("There was an error in exporting. Please contact developer.");
    }
    confirmMsg.exec();
}

/**
 * @brief Exports all tables to corresponding files on desktop
 * @param shiftDatabase&        reference to shiftDatabase object
 */
void importExporter::exportAll(shiftDatabase &dataB)
{
    exportCars(dataB);
    exportCustomers(dataB);
    exportRentals(dataB);
}
