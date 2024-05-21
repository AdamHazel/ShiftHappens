#include "importexporter.h"
#include "rapidcsv/rapidcsv.h"

#include <QMessageBox>
#include <fstream>


void importExporter::importCars(shiftDatabase &dataB)
{
    QMessageBox question;
    question.setWindowTitle("WARNING");
    question.setText("Importing cars will delete existing records.\nDo you wish to continue?");
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

            qDebug() << rowCount << " and col count is " << colCount;

            if (colCount != 5)
            {
                QMessageBox errorMsg;
                errorMsg.setWindowTitle("Error");
                errorMsg.setText("Error in amount of columns. \nThere should be five columns");
                errorMsg.setDetailedText("Columns are separated by commas. \nThere should be four commas and five column headers is the .csv file ");
                errorMsg.exec();
                return;
            }
            else
            {
                if (rowCount >= 1)
                {
                    size_t row { 0 };
                    while(row < rowCount)
                    {
                        car temp;
                        size_t col { 0 };
                        while(col < colCount)
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
                                temp.year = carDoc.GetCell<uint>(col,row);
                                break;
                            }
                            case 4:
                            {
                                temp.custId = carDoc.GetCell<uint>(col,row);
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
                    emptyWarning.setText("cars_import.csv appears to be empty. \nNo customers imported");
                    emptyWarning.exec();
                    return;
                }

                dataB.cars_resetTable();

                uint rowInsertCount {0};

                for (auto &car : allCars)
                {
                    if (dataB.cars_add(&car.RegNr,&car.brand,&car.model,&car.year))
                    {
                        //NEED SOMETHING HERE TO APPLY RENTALS
                        ++rowInsertCount;
                    }
                }

                qDebug() << rowInsertCount;

                QMessageBox confirmMsg;

                if (rowInsertCount < rowCount)
                {
                    confirmMsg.setWindowTitle("Import error");
                    confirmMsg.setText(QString::number(rowInsertCount) + " of " + QString::number(rowCount) + " rows inserted.");
                    confirmMsg.setDetailedText("Check your import file for the following:\n"
                                               "  - Is a RegNr repeated?\n"
                                               "  - Are all values separated with a comma (,)?\n"
                                               "  - Are all non-number values encased with \" \"\n"
                                               "  - Check that information is correct");
                }
                else if (rowInsertCount == rowCount)
                {
                    confirmMsg.setWindowTitle("Import confirmation");
                    confirmMsg.setText("Number of rows inserted: " + QString::number(rowInsertCount));
                }
                confirmMsg.exec();
            }
        }

    }
    else
        return;
}

void importExporter::importCustomers(shiftDatabase &dataB)
{
    QMessageBox question;
    question.setWindowTitle("WARNING");
    question.setText("Importing customers will delete existing records. New ID will be assigned.\nDo you wish to continue?");
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

            qDebug() << rowCount << " and col count is " << colCount;

            if (colCount != 4)
            {
                QMessageBox errorMsg;
                errorMsg.setWindowTitle("Error");
                errorMsg.setText("Error in amount of columns. \nThere should be four columns");
                errorMsg.setDetailedText("Columns are separated by commas. \nThere should be three commas and four column headers is the .csv file ");
                errorMsg.exec();
                return;
            }
            else
            {
                if (rowCount >= 1)
                {
                    size_t row { 0 };
                    while(row < rowCount)
                    {
                        customer temp;
                        size_t col { 0 };
                        while(col < colCount)
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
                                if (temp.postcode > 9999)
                                {
                                    QMessageBox errorMsg;
                                    errorMsg.setWindowTitle("Error");
                                    errorMsg.setText("Invalid postcode in file. \nPlease fix import file.");
                                    errorMsg.setDetailedText("Postcodes are between 0 and 9999.");
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
                    emptyWarning.setText("customers_import.csv appears to be empty. \nNo customers imported");
                    emptyWarning.exec();
                    return;
                }

                dataB.customer_resetTable();

                uint rowInsertCount {0};

                for (auto &cust : allCustomers)
                {
                    if (dataB.customer_add(&cust.name, &cust.street, &cust.postcode, &cust.city) == true)
                        ++rowInsertCount;
                }

                QMessageBox confirmMsg;

                if (rowInsertCount < rowCount)
                {
                    confirmMsg.setWindowTitle("Import error");
                    confirmMsg.setText(QString::number(rowInsertCount) + " of " + QString::number(rowCount) + " rows inserted.");
                    confirmMsg.setDetailedText("Check your import file for the following:\n"
                                               "  - Are all values separated with a comma (,)?\n"
                                               "  - Are all non-number values encased with \" \"\n"
                                               "  - Check that information is correct");
                }
                else if (rowInsertCount == rowCount)
                {
                    confirmMsg.setWindowTitle("Import confirmation");
                    confirmMsg.setText("Number of rows inserted: " + QString::number(rowInsertCount));
                }
                confirmMsg.exec();
            }
        }

    }
    else
        return;
}

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
        cars << "RegNr,brand,model,year,custId\n";
        cars.close();
    }

    QMessageBox confirmMsg;
    if (custTempAlreadyExists == true && carTempAlreadyExists == true)
    {
        confirmMsg.setWindowTitle("No templates created");
        confirmMsg.setText("Templates already exist.");
        confirmMsg.setDetailedText("Templates can be used to import cars and customers.\n"
                                   "File type is .csv. Use commas (,) to separate values.\n"
                                   "To import files, save files to desktop.\n"
                                   "Each entry should have a new row.\n"
                                   "Incase non-number values with \"\" .\n");
    } else if (custTempAlreadyExists == true && carTempAlreadyExists == false)
    {
        confirmMsg.setWindowTitle("Template created");
        confirmMsg.setText("Cars template created. Customer template already exists.");
        confirmMsg.setDetailedText("Templates can be used to import cars and customers.\n"
                                   "File type is .csv. Use commas (,) to separate values.\n"
                                   "To import files, save files to desktop.\n"
                                   "Each entry should have a new row.\n"
                                   "Incase non-number values with \"\" .\n");
    } else if (custTempAlreadyExists == false && carTempAlreadyExists == true)
    {
        confirmMsg.setWindowTitle("Template created");
        confirmMsg.setText("Customer template created. Cars template already exists.");
        confirmMsg.setDetailedText("Templates can be used to import cars and customers.\n"
                                   "File type is .csv. Use commas (,) to separate values.\n"
                                   "To import files, save files to desktop.\n"
                                   "Each entry should have a new row.\n"
                                   "Incase non-number values with \"\" .\n");
    }
    else
    {
        confirmMsg.setWindowTitle("Templates created");
        confirmMsg.setText("Templates saved to desktop.");
        confirmMsg.setDetailedText("Templates can be used to import cars and customers.\n"
                                   "File type is .csv. Use commas (,) to separate values.\n"
                                   "To import files, save files to desktop.\n"
                                   "Each entry should have a new row.\n"
                                   "Incase non-number values with \"\" .\n");
    }
    confirmMsg.exec();
}


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
        carsFile << "RegNr,brand,model,year,custId\n";

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

void importExporter::exportAll(shiftDatabase &dataB)
{
    exportCars(dataB);
    exportCustomers(dataB);
}
