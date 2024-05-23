#include "importexporter.h"
#include "helper_functions.h"
#include "rapidcsv/rapidcsv.h"


#include <QMessageBox>
#include <fstream>

/**
 * @brief Imports cars from a .csv file
 * @param shiftDatabase&        reference to shiftDatabase object
 */
void importExporter::importCars(shiftDatabase &dataB)
{
    QMessageBox question;
    question.setWindowTitle("WARNING");
    question.setText("Importing cars will delete existing records of cars and rentals.\nDo you wish to continue?");
    question.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    question.setDefaultButton(QMessageBox::Yes);
    int answer = question.exec();

    if (answer == QMessageBox::Yes)
    {
        QString carsimportPath = QString("%1/cars_import.csv")
                                     .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        QByteArray carsIP = carsimportPath.toLocal8Bit();
        const char* importFile = carsIP.data();

        if (!std::filesystem::exists(importFile))
        {
            QMessageBox errorMsg;
            errorMsg.setWindowTitle("Error");
            errorMsg.setText("File not found. \nFile should be saved on desktop");
            errorMsg.exec();
            return;
        }
        else
        {
            rapidcsv::Document carDoc(importFile);
            auto rowCount = carDoc.GetRowCount();
            auto colCount = carDoc.GetColumnCount();

            if (colCount != 4)
            {
                QMessageBox errorMsg;
                errorMsg.setWindowTitle("Error");
                errorMsg.setText("Error in amount of columns. \nThere should be four columns (RegNr,brand,model,year).");
                errorMsg.setDetailedText("Columns are separated by commas. \nThere should be three commas and four column headers is the .csv file. ");
                errorMsg.exec();
                return;
            }
            else
            {
                if (rowCount >= 1)
                {
                    size_t row {0};
                    allCars.clear();
                    while (row < rowCount)
                    {
                        car temp;
                        size_t col {0};
                        while (col < colCount)
                        {
                            switch (col)
                            {
                            case 0:
                            {
                                temp.RegNr = QString::fromStdString(carDoc.GetCell<std::string>(col,row)).toUpper();
                                if(temp.RegNr.length() > 7)
                                {
                                    QMessageBox errorMsg;
                                    errorMsg.setWindowTitle("Error");
                                    errorMsg.setText("Invalid registration number");
                                    errorMsg.setDetailedText("Registration numbers should be maximum 7 characters long. \nPlease fix import file.");
                                    errorMsg.exec();
                                    return;
                                }
                                break;
                            }
                            case 1:
                            {
                                temp.brand = QString::fromStdString(carDoc.GetCell<std::string>(col,row)).toUpper();
                                break;
                            }
                            case 2:
                            {
                                temp.model = QString::fromStdString(carDoc.GetCell<std::string>(col,row)).toUpper();
                                break;
                            }
                            case 3:
                            {
                                temp.dayPrice = carDoc.GetCell<uint>(col,row);
                                break;
                            }
                            }
                            ++col;
                        }
                        allCars.push_back(temp);
                        ++row;
                    }

                }
                else
                {
                    QMessageBox emptyWarning;
                    emptyWarning.setWindowTitle("Error");
                    emptyWarning.setText("cars_import.csv appears to be empty. \nNo cars imported.");
                    emptyWarning.exec();
                    return;
                }

                dataB.cars_resetTable();

                uint rowInsertCount {0};

                for (auto &car : allCars)
                {
                    qDebug() << "Car " << rowInsertCount << " is " << car.RegNr;
                    if (dataB.cars_add(&car.RegNr, &car.brand, &car.model, &car.dayPrice))
                        ++rowInsertCount;
                }

                allCars.clear();

                QMessageBox confirmMsg;
                if (rowInsertCount < rowCount)
                {
                    confirmMsg.setWindowTitle("Import error");
                    confirmMsg.setText(QString::number(rowInsertCount) + " of " + QString::number(rowCount) + " cars inserted.");
                    confirmMsg.setDetailedText("Check your import file for the following:\n"
                                               "  - Is a RegNr repeated?\n"
                                               "  - Are all values separated with a comma (,)?\n"
                                               "  - Are all non-number values encased with \" \"\n"
                                               "  - Check that information is correct");
                }
                else if (rowInsertCount == rowCount)
                {
                    confirmMsg.setWindowTitle("Import confirmation");
                    confirmMsg.setText("Number of cars inserted: " + QString::number(rowInsertCount));
                }
                confirmMsg.exec();
            }
        }
    }
    else
        return;
}

/**
 * @brief Imports customers from a .csv file
 * @param shiftDatabase&        reference to shiftDatabase object
 */
void importExporter::importCustomers(shiftDatabase &dataB)
{
    QMessageBox question;
    question.setWindowTitle("WARNING");
    question.setText("Importing customers will delete existing records of customers and rentals.\nDo you wish to continue?");
    question.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    question.setDefaultButton(QMessageBox::Yes);
    int answer = question.exec();

    if (answer == QMessageBox::Yes)
    {
        QString customersimportPath = QString("%1/customers_import.csv")
                                     .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        QByteArray custIP = customersimportPath.toLocal8Bit();
        const char* importFile = custIP.data();

        if (!std::filesystem::exists(importFile))
        {
            QMessageBox errorMsg;
            errorMsg.setWindowTitle("Error");
            errorMsg.setText("File not found. \nFile should be saved on desktop");
            errorMsg.exec();
            return;
        }
        else
        {
            rapidcsv::Document cusDoc(importFile);
            auto rowCount = cusDoc.GetRowCount();
            auto colCount = cusDoc.GetColumnCount();

            if (colCount != 4)
            {
                QMessageBox errorMsg;
                errorMsg.setWindowTitle("Error");
                errorMsg.setText("Error in amount of columns. \nThere should be four columns. (name,street,postcode,city)");
                errorMsg.setDetailedText("Columns are separated by commas. \nThere should be three commas and four column headers is the .csv file. ");
                errorMsg.exec();
                return;
            }
            else
            {
                if (rowCount >= 1)
                {
                    size_t row {0};
                    allCustomers.clear();
                    while (row < rowCount)
                    {
                        customer temp;
                        size_t col {0};
                        while (col < colCount)
                        {
                            switch (col)
                            {
                            case 0:
                            {
                                temp.name = QString::fromStdString(cusDoc.GetCell<std::string>(col,row)).toUpper();
                                break;
                            }
                            case 1:
                            {
                                temp.street = QString::fromStdString(cusDoc.GetCell<std::string>(col,row)).toUpper();
                                break;
                            }
                            case 2:
                            {
                                temp.postcode = cusDoc.GetCell<uint>(col,row);
                                if (temp.postcode > 9999 || temp.postcode < 1)
                                {
                                    QMessageBox errorMsg;
                                    errorMsg.setWindowTitle("Error");
                                    errorMsg.setText("Invalid postcode in file. \nPlease fix import file.");
                                    errorMsg.setDetailedText("Postcodes are between 1 and 9999.");
                                    errorMsg.exec();
                                    return;
                                }
                                break;
                            }
                            case 3:
                            {
                                temp.city = QString::fromStdString(cusDoc.GetCell<std::string>(col,row)).toUpper();
                                break;
                            }
                            }
                            ++col;
                        }
                        allCustomers.push_back(temp);
                        ++row;
                    }

                }
                else
                {
                    QMessageBox emptyWarning;
                    emptyWarning.setWindowTitle("Error");
                    emptyWarning.setText("customers_import.csv appears to be empty. \nNo customers imported.");
                    emptyWarning.exec();
                    return;
                }

                dataB.customer_resetTable();

                uint rowInsertCount {0};

                for (auto &customer : allCustomers)
                {

                    if (dataB.customer_add(&customer.name, &customer.street, &customer.postcode, &customer.city))
                        ++rowInsertCount;
                }

                allCustomers.clear();

                QMessageBox confirmMsg;
                if (rowInsertCount < rowCount)
                {
                    confirmMsg.setWindowTitle("Import error");
                    confirmMsg.setText(QString::number(rowInsertCount) + " of " + QString::number(rowCount) + " customers inserted.");
                    confirmMsg.setDetailedText("Check your import file for the following:\n"
                                               "  - Are all values separated with a comma (,)?\n"
                                               "  - Are all non-number values encased with \" \"\n"
                                               "  - Check that information is correct");
                }
                else if (rowInsertCount == rowCount)
                {
                    confirmMsg.setWindowTitle("Import confirmation");
                    confirmMsg.setText("Number of customers inserted: " + QString::number(rowInsertCount));
                }
                confirmMsg.exec();
            }
        }
    }
    else
        return;
}

/**
 * @brief Imports rentals from a .csv file
 * @param shiftDatabase&        reference to shiftDatabase object
 */
void importExporter::importRentals(shiftDatabase &dataB)
{
    QMessageBox question;
    question.setWindowTitle("WARNING");
    question.setText("Importing rentals will delete existing records.\nDo you wish to continue?");
    question.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    question.setDefaultButton(QMessageBox::Yes);
    int answer = question.exec();

    if (answer == QMessageBox::Yes)
    {
        QString rentalsimportPath = QString("%1/rentals_import.csv")
                                          .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        QByteArray rentalIP = rentalsimportPath.toLocal8Bit();
        const char* importFile = rentalIP.data();

        if (!std::filesystem::exists(importFile))
        {
            QMessageBox errorMsg;
            errorMsg.setWindowTitle("Error");
            errorMsg.setText("File not found. \nFile should be saved on desktop");
            errorMsg.exec();
            return;
        }
        else
        {
            rapidcsv::Document rentalDoc(importFile);
            auto rowCount = rentalDoc.GetRowCount();
            auto colCount = rentalDoc.GetColumnCount();

            if (colCount != 5)
            {
                QMessageBox errorMsg;
                errorMsg.setWindowTitle("Error");
                errorMsg.setText("Error in amount of columns. \nThere should be five columns (RegNr,custId,startDate,endDate,completed).");
                errorMsg.setDetailedText("Columns are separated by commas. \nThere should be four commas and five column headers is the .csv file. ");
                errorMsg.exec();
                return;
            }
            else
            {
                if (rowCount >= 1)
                {
                    size_t row {0};
                    allRentals.clear();
                    while (row < rowCount)
                    {
                        rental temp;
                        size_t col {0};
                        while (col < colCount)
                        {
                            switch (col)
                            {
                            case 0:
                            {
                                temp.RegNr = QString::fromStdString(rentalDoc.GetCell<std::string>(col,row)).toUpper();
                                break;
                            }
                            case 1:
                            {
                                temp.custId = rentalDoc.GetCell<uint>(col,row);
                                break;
                            }
                            case 2:
                            {
                                QString sDate = QString::fromStdString(rentalDoc.GetCell<std::string>(col,row)).toUpper();
                                temp.startDate = QDate::fromString(sDate, "dd-MM-yyyy");
                                break;
                            }
                            case 3:
                            {
                                QString eDate = QString::fromStdString(rentalDoc.GetCell<std::string>(col,row)).toUpper();
                                temp.endDate = QDate::fromString(eDate, "dd-MM-yyyy");
                                break;
                            }
                            case 4:
                            {
                                temp.completed = rentalDoc.GetCell<int>(col,row);
                                if (! (temp.completed == 0 || temp.completed == 1) )
                                {
                                    QMessageBox errorMsg;
                                    errorMsg.setWindowTitle("Error");
                                    errorMsg.setText("Invalid number given for completed. \nPlease fix import file.");
                                    errorMsg.setDetailedText("Completed can be 0 or 1 (0 for uncompleted, and 1 for completed).");
                                    errorMsg.exec();
                                    return;
                                }
                                break;
                            }
                            }
                            ++col;
                        }
                        allRentals.push_back(temp);
                        ++row;
                    }

                }
                else
                {
                    QMessageBox emptyWarning;
                    emptyWarning.setWindowTitle("Error");
                    emptyWarning.setText("rentals_import.csv appears to be empty. \nNo rentals imported.");
                    emptyWarning.exec();
                    return;
                }

                dataB.rental_resetTable();

                uint rowInsertCount {0};


                for (auto &rent : allRentals)
                {
                    if (rent.startDate < rent.endDate)
                    {
                        if (dataB.cars_checkExistence(rent.RegNr) && dataB.customer_checkExistence(rent.custId))
                        {
                            uint dayPrice = dataB.cars_fetchDayPrice(rent.RegNr);
                            rent.totalPrice = totalPriceCalculate(rent.startDate, rent.endDate, dayPrice);
                            QString sDate = rent.startDate.toString("dd-MM-yyyy");
                            QString eDate = rent.endDate.toString("dd-MM-yyyy");

                            if (rowInsertCount == 0)
                            {
                                if (dataB.rental_addRental(rent.RegNr, rent.custId, sDate, eDate, rent.totalPrice))
                                    ++rowInsertCount;
                            }
                            else
                            {
                                bool safetoAdd = true;
                                std::vector<rental> addedRentals = dataB.rental_FetchAll();
                                for (const auto added : addedRentals)
                                {
                                    if (rent.RegNr == added.RegNr)
                                    {
                                        if ( (rent.startDate <= added.startDate) && (rent.endDate >= added.startDate) && (rent.endDate <= added.endDate) )
                                        {
                                            safetoAdd = false;
                                            break;
                                        }

                                        if ( (rent.startDate >= added.startDate) && (rent.startDate <= added.endDate) && (rent.endDate >= added.endDate))
                                        {
                                            safetoAdd = false;
                                            break;
                                        }

                                        if ( (rent.startDate >= added.startDate) && (rent.endDate <= added.endDate))
                                        {
                                            safetoAdd = false;
                                            break;
                                        }

                                        if ( (rent.startDate <= added.startDate) && (rent.endDate >= added.endDate))
                                        {
                                            safetoAdd = false;
                                            break;
                                        }
                                    }
                                }
                                if (safetoAdd == true)
                                {
                                    if (dataB.rental_addRental(rent.RegNr, rent.custId, sDate, eDate, rent.totalPrice))
                                        ++rowInsertCount;
                                }
                            }
                        }
                    }
                }

                QMessageBox confirmMsg;
                if (rowInsertCount < rowCount)
                {
                    confirmMsg.setWindowTitle("Import error");
                    confirmMsg.setText(QString::number(rowInsertCount) + " of " + QString::number(rowCount) + " rentals inserted.");
                    confirmMsg.setDetailedText("Check your import file for the following:\n"
                                               "  - Does the customer exist?\n"
                                               "  - Does the car exist?\n"
                                               "  - Are all values separated with a comma (,)?\n"
                                               "  - Are all non-number values encased with \" \"\n"
                                               "  - Check that information is correct");
                }
                else if (rowInsertCount == rowCount)
                {
                    confirmMsg.setWindowTitle("Import confirmation");
                    confirmMsg.setText("Number of rentals inserted: " + QString::number(rowInsertCount));
                }
                confirmMsg.exec();
            }
        }
    }
    else
        return;
}


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
        rentals << "RegNr,custId,startDate,endDate,completed\n";
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
        allCustomers.clear();
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
        allCars.clear();
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
        allRentals.clear();
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
