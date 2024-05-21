#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcar_d.h"
#include "editcar_d.h"
#include "addcustomer_d.h"
#include "editcustomer_d.h"
#include "addrental_d.h"

#include <QPushButton>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataB.cars_createTable();
    dataB.customer_createTable();
    dataB.rentals_createTable();
    viewAll();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Displays customers from database in tableview on main window
 */
void MainWindow::viewCustomers()
{
    QSqlTableModel *customerTable = new QSqlTableModel(this);
    customerTable->setTable("customers");
    customerTable->setHeaderData(0, Qt::Horizontal, "Customer Id");
    customerTable->setHeaderData(1, Qt::Horizontal, "Name");
    customerTable->setHeaderData(2, Qt::Horizontal, "Street");
    customerTable->setHeaderData(3, Qt::Horizontal, "Postcode");
    customerTable->setHeaderData(4, Qt::Horizontal, "City");
    customerTable->select();

    ui->tableView_customers->setModel(customerTable);
    ui->tableView_customers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_customers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_customers->resizeColumnsToContents();
    ui->tableView_customers->show();
}

/**
 * @brief Displays cars from database in tableview on main window
 */
void MainWindow::viewCars(){
    QSqlTableModel *carsTable = new QSqlTableModel(this);
    carsTable->setTable("cars");
    carsTable->setHeaderData(0, Qt::Horizontal, "Car Reg.Nr");
    carsTable->setHeaderData(1, Qt::Horizontal, "Brand");
    carsTable->setHeaderData(2, Qt::Horizontal, "Model");
    carsTable->setHeaderData(3, Qt::Horizontal, "Day Price (kr)");
    carsTable->select();

    ui->tableView_cars->setModel(carsTable);
    ui->tableView_cars->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_cars->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_cars->resizeColumnsToContents();
    ui->tableView_cars->show();
}

/**
 * @brief Displays rentals from database in tableview on main window
 */
void MainWindow::viewRentals(){
    QSqlTableModel *rentalsTable = new QSqlTableModel(this);
    rentalsTable->setTable("rentals");
    rentalsTable->setHeaderData(0, Qt::Horizontal, "Rental Id");
    rentalsTable->setHeaderData(1, Qt::Horizontal, "Car Reg.Nr");
    rentalsTable->setHeaderData(2, Qt::Horizontal, "Customer ID");
    rentalsTable->setHeaderData(3, Qt::Horizontal, "Start date");
    rentalsTable->setHeaderData(4, Qt::Horizontal, "End date");
    rentalsTable->setHeaderData(5, Qt::Horizontal, "Total Price (kr)");
    rentalsTable->setHeaderData(6, Qt::Horizontal, "Completed");
    rentalsTable->select();

    ui->tableView_rentals->setModel(rentalsTable);
    ui->tableView_rentals->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_rentals->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_rentals->resizeColumnsToContents();
    ui->tableView_rentals->show();
}

/**
 * @brief Displays all tables from database
 */
void MainWindow::viewAll() {
    viewCustomers();
    viewCars();
    viewRentals();
}


// Slots
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


void MainWindow::on_addRentalB_clicked()
{
    addrental_d dialog(this);
    dialog.exec();
    viewRentals();
}


void MainWindow::on_removeRentalB_clicked()
{
    QModelIndex selectedRow = ui->tableView_rentals->currentIndex();
    if (selectedRow.row() == -1) {
        QMessageBox::information(this, "Selection Needed", "Please select a rental to remove.");
        return;
    }
    if (QMessageBox::Yes == QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this rental?",
                                                  QMessageBox::Yes | QMessageBox::No)) {
        QSqlTableModel *tableModel = qobject_cast<QSqlTableModel*>(ui->tableView_rentals->model());
        uint rentalId = tableModel->record(selectedRow.row()).value("id").toUInt();
        if (dataB.rental_removeRental(rentalId)) {
            QMessageBox::information(this, "Success", "Rental removed!");
            tableModel->select();
        } else {
            QMessageBox::critical(this, "Error", "Failed to remove rental.");
        }
    }
    viewRentals();
}

// WORK HERE
void MainWindow::on_pushButton_clicked()
{
    /*
    QModelIndex selectedRow = ui->tableView_rentals->currentIndex();
    if (selectedRow.row() == -1) {
        QMessageBox::information(this, "Selection Needed", "Please select a rental to update.");
        return;
    }
    if (QMessageBox::Yes == QMessageBox::question(this, "Confirm Update", "Are you sure you want to update this rental?",
                                                  QMessageBox::Yes | QMessageBox::No)) {
        QSqlTableModel *tableModel = qobject_cast<QSqlTableModel*>(ui->tableView_rentals->model());
        uint rentalId = tableModel->record(selectedRow.row()).value("id").toUInt();
        if (dataB.rental_completeRental(rentalId)) {
            QMessageBox::information(this, "Success", "Rental updated!");
            tableModel->select();
        } else {

        }
    }
    viewRentals();
*/
}

