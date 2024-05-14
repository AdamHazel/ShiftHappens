#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcar_d.h"
#include "addcustomer_d.h"
#include "editcar_d.h"
#include "editcustomer_d.h"
#include "importExport.h"

#include <QPushButton>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataB.cars_createTable();
    dataB.customer_createTable();
    viewCars();
    viewCustomers();

    // Connectors:
    // connect(ui->addCarB, &QPushButton::clicked, this, &MainWindow::on_addCarB_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addCarB_clicked()
{
    addCar_D dialog(this);
    dialog.exec();
    viewCars();
}

void MainWindow::on_editCarB_clicked()
{
    QModelIndex selectedRow = ui->tableView_cars->currentIndex();
    if (selectedRow.row() == -1) {
        QMessageBox::information(this, "Selection Needed", "Please select a car to edit.");
        return;
    }
    QString regNr = ui->tableView_cars->model()->data(ui->tableView_cars->model()->index(selectedRow.row(), regNrColumn)).toString();
    editCar_D dialog(regNr, this);
    dialog.exec();
    viewCars();
}

void MainWindow::on_removeCarB_clicked()
{
    QModelIndex selectedRow = ui->tableView_cars->currentIndex();
    if (selectedRow.row() == -1) {
        QMessageBox::information(this, "Selection Needed", "Please select a car to remove.");
        return;
    }
    if (QMessageBox::Yes == QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this car?",
                                                  QMessageBox::Yes | QMessageBox::No)) {
        QSqlTableModel *tableModel = qobject_cast<QSqlTableModel*>(ui->tableView_cars->model());

        QString RegNr = tableModel->record(selectedRow.row()).value("RegNr").toString();

        if (dataB.cars_removeByRegNr(&RegNr)) {
            QMessageBox::information(this, "Success", "Car removed!");
            tableModel->select();
        } else {
            QMessageBox::critical(this, "Error", "Failed to remove the car.");
        }
    }
    viewCars();
}

void MainWindow::on_addCustB_clicked()
{
    addCustomer_D dialog(this);
    dialog.exec();
    viewCustomers();
}

void MainWindow::on_editCustB_clicked()
{
    QModelIndex selectedRow = ui->tableView_customers->currentIndex();
    if (selectedRow.row() == -1) {
        QMessageBox::information(this, "Selection Needed", "Please select a customer to edit.");
        return;
    }
    uint id = ui->tableView_customers->model()->data(ui->tableView_customers->model()->index(selectedRow.row(), IdColumn)).toUInt();
    editCustomer_D dialog(id, this);
    dialog.exec();
    viewCustomers();
}

void MainWindow::on_removeCustB_clicked()
{
    QModelIndex selectedRow = ui->tableView_customers->currentIndex();
    if (selectedRow.row() == -1) {
        QMessageBox::information(this, "Selection Needed", "Please select a customer to remove.");
        return;
    }
    if (QMessageBox::Yes == QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this customer?",
                                                  QMessageBox::Yes | QMessageBox::No)) {
        QSqlTableModel *tableModel = qobject_cast<QSqlTableModel*>(ui->tableView_customers->model());
        uint customerId = tableModel->record(selectedRow.row()).value("id").toUInt();
        if (dataB.customer_removeByID(&customerId)) {
            QMessageBox::information(this, "Success", "Customer removed!");
            tableModel->select();
        } else {
            QMessageBox::critical(this, "Error", "Failed to remove customer.");
        }
    }
    viewCustomers();
}


void MainWindow::viewCustomers(){
    QSqlTableModel *customerTable = new QSqlTableModel(this);
    customerTable->setTable("customers");
    customerTable->select();

    ui->tableView_customers->setModel(customerTable);
    ui->tableView_customers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_customers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_customers->resizeColumnsToContents();
    ui->tableView_customers->show();
}
void MainWindow::viewCars(){
    QSqlTableModel *carsTable = new QSqlTableModel(this);
    carsTable->setTable("cars");
    carsTable->select();

    ui->tableView_cars->setModel(carsTable);
    ui->tableView_cars->setColumnHidden(4, true);
    ui->tableView_cars->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_cars->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_cars->resizeColumnsToContents();
    ui->tableView_cars->show();
}
void MainWindow::viewRentals(){
    QSqlTableModel *rentalsTable = new QSqlTableModel(this);
    rentalsTable->setTable("cars");
    rentalsTable->select();

    ui->tableView_rentals->setModel(rentalsTable);
    ui->tableView_cars->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_cars->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_cars->resizeColumnsToContents();
    ui->tableView_cars->show();
}
void MainWindow::viewAssignedCar(){
    QSqlTableModel *assignCarsTable = new QSqlTableModel(this);
    assignCarsTable->setTable("cars");
    assignCarsTable->select();

    ui->tableView_rentals->setModel(assignCarsTable);
    ui->tableView_cars->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_cars->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_cars->resizeColumnsToContents();
    ui->tableView_cars->show();
}
void MainWindow::viewAssignCustomer(){
    QSqlTableModel *assignCustomerTable = new QSqlTableModel(this);
    assignCustomerTable->setTable("customers");
    assignCustomerTable->select();

    ui->tableView_rentals->setModel(assignCustomerTable);
    ui->tableView_cars->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_cars->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_cars->resizeColumnsToContents();
    ui->tableView_cars->show();
}

void MainWindow::on_actionExit_triggered()
{
    if
        (QMessageBox::Yes == QMessageBox::question(this, "Confirmation", "Are you sure you want to exit the program?", QMessageBox::Yes | QMessageBox::No))
        std::exit(0);
    else
        return;
}


void MainWindow::on_actionImport_customers_triggered()
{
    importCustomers();
    viewCustomers();
}


void MainWindow::on_actionImport_cars_triggered()
{
    importCars();
    viewCars();
}


void MainWindow::on_actionCreate_templates_triggered()
{
    // Customers template
    QString customersdesktopPath = QString("%1/customers_import.csv")
                                       .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    QByteArray custDP = customersdesktopPath.toLocal8Bit();
    const char* custPath = custDP.data();

    std::ofstream customers;
    customers.open(custPath);
    customers << "name,street,postcode,city\n";
    customers.close();

    // Cars template
    QString carsdesktopPath = QString("%1/cars_import.csv")
                                  .arg(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    QByteArray carsDP = carsdesktopPath.toLocal8Bit();
    const char* carsPath = carsDP.data();

    std::ofstream cars;
    cars.open(carsPath);
    cars << "RegNr,brand,model,year,custId\n";
    cars.close();

    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle("Templates created");
    confirmMsg.setText("Templates saved to desktop.");
    confirmMsg.setDetailedText("Templates can be used to import cars and customers.\n"
                               "File type is .csv. Use commas (,) to separate values.\n"
                               "To import files, save files to desktop.\n"
                               "Each entry should have a new row.\n"
                               "Incase non-number values with \"\" .\n");
    confirmMsg.exec();
}


void MainWindow::on_actionExport_customers_triggered()
{
    auto count = exportCustomers();

    if (count > 0 )
    {
        QMessageBox confirmMsg;
        confirmMsg.setWindowTitle("Export confirmation");
        confirmMsg.setText("Number of customers exported: " + QString::number(count));
        confirmMsg.exec();
    }
}


void MainWindow::on_actionExport_cars_triggered()
{
    auto count = exportCars();

    if (count > 0 )
    {
        QMessageBox confirmMsg;
        confirmMsg.setWindowTitle("Export confirmation");
        confirmMsg.setText("Number of customers exported: " + QString::number(count));
        confirmMsg.exec();
    }
}


void MainWindow::on_actionExport_all_triggered()
{
    auto results = exportAll();

    QMessageBox confirmMsg;
    confirmMsg.setWindowTitle("Export confirmation");

    confirmMsg.setText("Number of customers exported: " + QString::number(results.first) + "\n"
                                                                                           "Number of cars exported: " + QString::number(results.second));
    confirmMsg.exec();
}

