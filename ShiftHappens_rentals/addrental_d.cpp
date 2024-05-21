#include "addrental_d.h"
#include "ui_addrental_d.h"

addrental_d::addrental_d(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addrental_d)
{
    ui->setupUi(this);
    viewCustomers_rental();
    viewCars_rental();
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
    QString searchText = ui->lineEdit_customerSearch->text().toUpper();
    QString name = "name LIKE '%"+ searchText + "%'";
    QString street = "street LIKE '%" + searchText + "%'";
    QString city = "city LIKE '%" + searchText + "%'";
    //QString id = "id = " + searchText;
    QString Filter = QString("%1 OR %2 OR %3").arg(name, street, city);

    customerTable->setFilter(Filter);
    customerTable->select();

}


void addrental_d::on_pushButton_carSearch_clicked()
{
    QString searchText = ui->lineEdit_carSearch->text().toUpper();
    QString RegNr = "RegNr LIKE '%" + searchText + "%'";
    QString brand = "brand LIKE '%" + searchText + "%'";
    QString model = "model LIKE '%" + searchText + "%'";
    QString Filter = QString("%1 OR %2 OR %3").arg(RegNr, brand, model);

    carsTable->setFilter(Filter);
    carsTable->select();
}


void addrental_d::on_pushButton_reset_clicked()
{
    ui->lineEdit_customerSearch->clear();
    ui->lineEdit_carSearch->clear();
    customerTable->setFilter("");
    carsTable->setFilter("");
    customerTable->select();
    carsTable->select();
}


void addrental_d::on_buttonBox_accepted()
{
    QModelIndex carIndex = ui->tableView_assignCustomer->currentIndex();
    QModelIndex custIndex = ui->tableView_assignCar->currentIndex();

    //bool rental_addRental(QString* RegNr, uint* custId, QString* startDate, QString* endDate, uint* totalPrice);
    QString RegNr = carsTable->data(carsTable->index(carIndex.row(), 0)).toString();
    qDebug() << "RegNr is " << RegNr;
    uint custId = customerTable->data(customerTable->index(custIndex.row(), 0)).toUInt();
    qDebug() << "RegNr is " << RegNr;
    QDate start = ui->dateEdit_start->date();
    QDate end = ui->dateEdit_end->date();
    QString startDate = start.toString();
    qDebug() << "startDate is " << startDate;
    QString endDate = end.toString();
    qDebug() << "endDate is " << endDate;
    uint dayCount { 1 };
    while (start != end)
    {
        start = start.addDays(1);
        ++dayCount;
    }
    uint dayPrice = carsTable->data(carsTable->index(carIndex.row(), 3)).toUInt();
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

