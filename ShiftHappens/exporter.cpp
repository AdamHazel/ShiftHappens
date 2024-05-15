#include "exporter.h"

#include <QMessageBox>
#include <fstream>

void exporter::exportCustomers(shiftDatabase &dataB)
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

void exporter::exportCars(shiftDatabase &dataB)
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
        carsFile << "id,name,street,postcode,city\n";

        for (auto &car : allCars)
        {
            if (carsFile.is_open())
            {
                carsFile << "\"" << car.RegNr.toStdString() << "\",\"" << car.brand.toStdString() << "\",\"" << car.model.toStdString()
                     << "\"," << car.year << "," << car.custId << "\n";
                ++carExportCount;
            }

        }
        qDebug() << "Amount of rows added to cars.csv: " << carExportCount;
        carsFile.close();
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

void exporter::exportAll(shiftDatabase &dataB)
{
    exportCars(dataB);
    exportCustomers(dataB);
}
