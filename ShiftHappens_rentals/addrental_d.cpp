#include "addrental_d.h"
#include "ui_addrental_d.h"
#include "rental.h"

#include <vector>

addrental_d::addrental_d(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addrental_d)
{
    ui->setupUi(this);
    viewCustomers_rental();
    viewCars_rental();

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

addrental_d::~addrental_d()
{
    delete ui;
}

void addrental_d::viewCars_rental(){
    carsTable->setTable("cars");
    carsTable->select();

    ui->tableView_assignCar->setModel(carsTable);
    ui->tableView_assignCar->setColumnHidden(3, true);
    ui->tableView_assignCar->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_assignCar->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_assignCar->resizeColumnsToContents();
    ui->tableView_assignCar->show();
}

void addrental_d::viewCustomers_rental(){
    customerTable->setTable("customers");
    customerTable->select();

    ui->tableView_assignCustomer->setModel(customerTable);
    ui->tableView_assignCustomer->setColumnHidden(4, true);
    ui->tableView_assignCustomer->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_assignCustomer->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_assignCustomer->resizeColumnsToContents();
    ui->tableView_assignCustomer->show();
}

void addrental_d::on_pushButton_customerSearch_clicked()
{
    dataB.databaseOpen();
    QString searchText = ui->lineEdit_customerSearch->text().toUpper();
    QString name = "name LIKE '%"+ searchText + "%'";
    QString street = "street LIKE '%" + searchText + "%'";
    QString city = "city LIKE '%" + searchText + "%'";
    //QString id = "id = " + searchText;
    QString Filter = QString("%1 OR %2 OR %3").arg(name, street, city);

    customerTable->setFilter(Filter);
    customerTable->select();
    dataB.databaseClose();
}


void addrental_d::on_pushButton_carSearch_clicked()
{
    dataB.databaseOpen();
    QString searchText = ui->lineEdit_carSearch->text().toUpper();
    QString RegNr = "RegNr LIKE '%" + searchText + "%'";
    QString brand = "brand LIKE '%" + searchText + "%'";
    QString model = "model LIKE '%" + searchText + "%'";
    QString Filter = QString("%1 OR %2 OR %3").arg(RegNr, brand, model);

    carsTable->setFilter(Filter);
    carsTable->select();
    dataB.databaseClose();
}


void addrental_d::on_pushButton_reset_clicked()
{
    dataB.databaseOpen();
    ui->lineEdit_customerSearch->clear();
    ui->lineEdit_carSearch->clear();
    customerTable->setFilter("");
    carsTable->setFilter("");
    customerTable->select();
    carsTable->select();

    ui->tableView_assignCustomer->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_assignCar->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_assignCustomer->show();
    ui->tableView_assignCar->show();

    dataB.databaseClose();
}


void addrental_d::on_buttonBox_accepted()
{
    QModelIndex carIndex = ui->tableView_assignCar->currentIndex();
    QModelIndex custIndex = ui->tableView_assignCustomer->currentIndex();

    QString RegNr = carsTable->data(carsTable->index(carIndex.row(), 0)).toString();
    uint custId = customerTable->data(customerTable->index(custIndex.row(), 0)).toUInt();

    QDate start = ui->dateEdit_start->date();
    QDate end = ui->dateEdit_end->date();
    QString startDate = start.toString("dd-MM-yyyy");
    QString endDate = end.toString("dd-MM-yyyy");

    uint dayCount { 1 };
    while (start != end)
    {
        start = start.addDays(1);
        ++dayCount;
    }
    uint dayPrice = carsTable->record(carIndex.row()).value("dayPrice").toUInt();
    uint totalPrice = dayCount * dayPrice;
    qDebug() << "totalprice is " << totalPrice;

    bool confirm = dataB.rental_addRental(RegNr, custId, startDate, endDate, totalPrice);
    QMessageBox confirmation;
    if (confirm == true)
    {
        confirmation.setWindowTitle("Confirmation");
        confirmation.setText("Successfully added rental");
    }
    else
    {
        confirmation.setWindowTitle("Error");
        confirmation.setText("Error adding rental");
    }
    confirmation.exec();

}


void addrental_d::on_pushButton_checkRental_clicked()
{
    QModelIndex selectCust = ui->tableView_assignCustomer->currentIndex();
    QModelIndex selectCar = ui->tableView_assignCar->currentIndex();

    if (selectCust.row() == -1 || selectCar.row() == -1)
    {
        QMessageBox::information(this, "Selections needed", "Please select both customer and car.");
        return;
    }

    QSqlTableModel *table = qobject_cast<QSqlTableModel*>(ui->tableView_assignCar->model());

    QString regN = table->record(selectCar.row()).value("RegNr").toString();
    QDate start = ui->dateEdit_start->date();
    QDate end = ui->dateEdit_end->date();

    if (start <= end)
    {
        bool safetoAdd = true;
        auto rentalCount = dataB.rental_countRecords();

        if (rentalCount > 0)
        {
            std::vector<rental> rentals = dataB.rental_FetchAll();
            for (const auto rent : rentals)
            {
                if (regN == rent.RegNr)
                {
                    if ( (start <= rent.startDate) && (end >= rent.startDate) && (end <= rent.endDate) )
                    {
                        safetoAdd = false;
                        break;
                    }

                    if ( (start >= rent.startDate) && (start <= rent.endDate) && (start >= rent.endDate))
                    {
                        safetoAdd = false;
                        break;
                    }

                    if ( (start >= rent.startDate) && (end <= rent.endDate))
                    {
                        safetoAdd = false;
                        break;
                    }

                    if ( (start <= rent.startDate) && (end >= rent.endDate))
                    {
                        safetoAdd = false;
                        break;
                    }
                }
            }
        }

        if (safetoAdd == true)
        {
            QMessageBox confirmation;
            confirmation.setWindowTitle("Checking rental");
            confirmation.setText("Car is available to assign to customer. Press ok to complete assignment");
            confirmation.exec();
            uint dayPrice = table->record(selectCar.row()).value("dayPrice").toUInt();
            uint dayCount { 1 };
            while (start != end)
            {
                start = start.addDays(1);
                ++dayCount;
            }
            uint totalPrice = dayPrice * dayCount;
            ui->spinBox_totalPrice->setValue(totalPrice);
            ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
            ui->pushButton_reset->setEnabled(false);
            ui->dateEdit_start->setReadOnly(true);
            ui->dateEdit_end->setReadOnly(true);
            return;
        }
        else
        {
            QMessageBox resultMessage;
            resultMessage.setWindowTitle("Checking rental");
            resultMessage.setText("Car is already assigned at this time");
            resultMessage.exec();
            return;
        }
    }
    else
    {
        QMessageBox resultMessage;
        resultMessage.setWindowTitle("Checking rental");
        resultMessage.setText("End date is before start date");
        resultMessage.exec();
        return;
    }


}


void addrental_d::on_dateEdit_start_userDateChanged(const QDate &date)
{
    ui->dateEdit_end->setDate(date);
}

