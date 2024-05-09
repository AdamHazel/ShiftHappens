#include "importExport.h"
#include "shiftdatabase.h"
#include "car.h"
#include "customer.h"

#include "rapidcsv/rapidcsv.h"

#include <QMessageBox>
#include <fstream>



int exportCustomers()
{
    uint customerExportCount { 0 };
    shiftDatabase db;

    if (db.customer_countEntries() == 0)
    {
        QMessageBox noFileMsg;
        noFileMsg.setWindowTitle("Export confirmation");
        noFileMsg.setText("Customers table is empty. No file was generated.");
        noFileMsg.exec();
        return customerExportCount;
    }

    auto allCustomers = db.customer_fetchAll();

    QString customersdesktopPath = QString("%1/customers.csv")
                                       .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    QByteArray custDP = customersdesktopPath.toLocal8Bit();
    const char* custPath = custDP.data();

    std::ofstream customers;
    customers.open(custPath);
    customers << "id,name,street,postcode,city\n";

    for (auto &cust : allCustomers)
    {
        if (customers.is_open())
        {
            customers << cust.id << ",\"" << cust.name.toStdString() << "\",\"" << cust.street.toStdString() << "\","
                      << cust.postcode << ",\"" << cust.city.toStdString() << "\"\n";
            ++customerExportCount;
        }

    }
    qDebug() << "Amount of rows added to customer.csv: " << customerExportCount;
    customers.close();

    return customerExportCount;
}

int exportCars()
{
    uint carExportCount { 0 };

    shiftDatabase db;
    if (db.cars_countEntries() == 0)
    {
        QMessageBox noFileMsg;
        noFileMsg.setWindowTitle("Export confirmation");
        noFileMsg.setText("Cars table is empty. \nNo export file for cars was generated.");
        noFileMsg.exec();
        return carExportCount;
    }
    auto allCars = db.cars_fetchAll();

    QString carsdesktopPath = QString("%1/cars.csv")
                                  .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    QByteArray carsDP = carsdesktopPath.toLocal8Bit();
    const char* carsPath = carsDP.data();

    std::ofstream cars;
    cars.open(carsPath);
    cars << "RegNr,brand,model,year,custId\n";

    for (auto &car : allCars)
    {
        if(cars.is_open())
        {
            cars << "\"" << car.RegNr.toStdString() << "\",\"" << car.brand.toStdString() << "\",\"" << car.model.toStdString()
                 << "\"," << car.year << "," << car.custId << "\n";
            ++carExportCount;;
        }
    }
    qDebug() << "Amount of rows added to cars.csv: " << carExportCount;

    cars.close();

    return carExportCount;
}

std::pair<int,int> exportAll()
{
    auto custCount = exportCustomers();
    auto carsCount = exportCars();

    std::pair<int,int> results (custCount,carsCount);

    return results;
}

void importCustomers()
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
            std::vector<customer> customers;
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
                        customers.push_back(temp);
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

                shiftDatabase db;
                db.customer_resetTable();

                uint rowInsertCount {0};

                for (auto &cust : customers)
                {
                    if (db.customer_add(&cust.name, &cust.street, &cust.postcode, &cust.city) == true)
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

void importCars()
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
            std::vector<car> cars;
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
                        cars.push_back(temp);
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

                shiftDatabase db;
                db.cars_resetTable();

                uint rowInsertCount {0};

                for (auto &car : cars)
                {
                    if (db.cars_add(&car.RegNr,&car.brand,&car.model,&car.year))
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


