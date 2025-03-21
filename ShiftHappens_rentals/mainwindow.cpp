#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcar_d.h"
#include "editcar_d.h"
#include "addcustomer_d.h"
#include "editcustomer_d.h"
#include "addrental_d.h"
#include "customer.h"

#include <QPushButton>
#include <fstream>

/**
 * @brief Constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataB.cars_createTable();
    dataB.customer_createTable();
    dataB.rental_createTable();
    viewAll();
}

/**
 * @brief Deconstructor
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Displays customers from database in tableview on main window
 */
void MainWindow::viewCustomers()
{
    dataB.databaseOpen();
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
    dataB.databaseClose();
}

/**
 * @brief Displays cars from database in tableview on main window
 */
void MainWindow::viewCars(){
    dataB.databaseOpen();
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
    dataB.databaseClose();
}

/**
 * @brief Displays rentals from database in tableview on main window
 */
void MainWindow::viewRentals(){
    dataB.databaseOpen();
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
    dataB.databaseClose();
}

/**
 * @brief Displays all tables from database
 */
void MainWindow::viewAll() {
    viewCustomers();
    viewCars();
    viewRentals();
}


/**
 * @brief Opens addCar dialog when user clicks appropriate button
 */
void MainWindow::on_addCarB_clicked()
{
    addCar_D dialog(this);
    dialog.exec();
    viewCars();
}

/**
 * @brief Opens editCar dialog when user clicks appropriate button
 */
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
    viewRentals();
}

/**
 * @brief Triggers removal of car and appropiate rentals based on user choice
 */
void MainWindow::on_removeCarB_clicked()
{
    QModelIndex selectedRow = ui->tableView_cars->currentIndex();
    if (selectedRow.row() == -1) {
        QMessageBox::information(this, "Selection Needed", "Please select a car to remove.");
        return;
    }
    if (QMessageBox::Yes == QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this car?"
                                                                          " Uncompleted rentals using this car will"
                                                                          " also be deleted.",
                                                  QMessageBox::Yes | QMessageBox::No)) {
        QSqlTableModel *tableModel = qobject_cast<QSqlTableModel*>(ui->tableView_cars->model());

        QString RegNr = tableModel->record(selectedRow.row()).value("RegNr").toString();

        if (dataB.cars_removeByRegNr(&RegNr))
        {
            dataB.rental_removeRentalonCar(RegNr);
            QMessageBox::information(this, "Success", "Car removed!");
            tableModel->select();
        } else {
            QMessageBox::critical(this, "Error", "Failed to remove the car.");
        }
    }
    viewCars();
    viewRentals();
}

/**
 * @brief Opens addCustomer dialog when user clicks appropriate button
 */
void MainWindow::on_addCustB_clicked()
{
    addCustomer_D dialog(this);
    dialog.exec();
    viewCustomers();
}

/**
 * @brief Opens editCustomer dialog when user clicks appropriate button
 */
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

/**
 * @brief Triggers removal of customer and appropiate rentals based on user choice
 */
void MainWindow::on_removeCustB_clicked()
{
    QModelIndex selectedRow = ui->tableView_customers->currentIndex();
    if (selectedRow.row() == -1) {
        QMessageBox::information(this, "Selection Needed", "Please select a customer to remove.");
        return;
    }
    if (QMessageBox::Yes == QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this customer?"
                                                                          " Uncompleted rentals assigned to this customer"
                                                                          " will also be deleted.",
                                                  QMessageBox::Yes | QMessageBox::No)) {
        QSqlTableModel *tableModel = qobject_cast<QSqlTableModel*>(ui->tableView_customers->model());
        uint customerId = tableModel->record(selectedRow.row()).value("id").toUInt();
        if (dataB.customer_removeByID(&customerId))
        {
            dataB.rental_removeRentalonCustomer(customerId);
            QMessageBox::information(this, "Success", "Customer removed!");
            tableModel->select();
        } else {
            QMessageBox::critical(this, "Error", "Failed to remove customer.");
        }
    }
    viewCustomers();
    viewRentals();
}

/**
 * @brief Opens addRental dialog when user clicks appropriate button
 */
void MainWindow::on_addRentalB_clicked()
{
    addrental_d dialog(this);
    dialog.exec();
    viewRentals();
}

/**
 * @brief Removal of rental based on user choice
 */
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
            QMessageBox::information(this, "Success", "Rental is removed.");
            tableModel->select();
        } else {
            QMessageBox::critical(this, "Error", "Failed to remove rental.");
        }
    }
    viewRentals();
}


/**
 * @brief Completion of rental based on user choice
 */
void MainWindow::on_pushButton_rentalCompleted_clicked()
{
    QModelIndex selectedRow = ui->tableView_rentals->currentIndex();
    if (selectedRow.row() == -1) {
        QMessageBox::information(this, "Selection Needed", "Please select a rental to set as completed.");
        return;
    }

    QSqlTableModel *tableModel = qobject_cast<QSqlTableModel*>(ui->tableView_rentals->model());
    int confirm = tableModel->record(selectedRow.row()).value("completed").toInt();
    if(confirm == 0)
    {
        QString eDate = tableModel->record(selectedRow.row()).value("endDate").toString();
        QDate endDate = QDate::fromString(eDate, "dd-MM-yyyy");
        QDate current = QDate::currentDate();

        if (endDate < current)
        {
            if (QMessageBox::Yes == QMessageBox::question(this, "Confirm completion", "Are you sure you want to complete this rental? A completed"
                                                                                      " rental cannot be reversed.",
                                                          QMessageBox::Yes | QMessageBox::No)) {
                uint rentalId = tableModel->record(selectedRow.row()).value("id").toUInt();
                if (dataB.rental_completeRental(rentalId)) {
                    QMessageBox::information(this, "Success", "Rental completed!");
                    tableModel->select();
                } else {
                    QMessageBox::critical(this, "Error", "Failed to complete rental.");
                }
            }
        }
        else
        {
            QMessageBox errorMessage;
            errorMessage.setWindowTitle("Error");
            errorMessage.setText("Cannot complete rental as rental is not over");
            errorMessage.exec();
        }
    }
    else
    {
        QMessageBox errorMessage;
        errorMessage.setWindowTitle("Error");
        errorMessage.setText("Rental is already completed");
        errorMessage.exec();
    }
    viewRentals();
}

/**
 * @brief Exits program
 */
void MainWindow::on_actionE_xit_triggered()
{
    if (QMessageBox::Yes == QMessageBox::question(this, "Exiting", "Are you sure you want to exit the program?", QMessageBox::Yes | QMessageBox::No))
        std::exit(0);
    else
        return;
}


void MainWindow::on_actionImport_Cars_triggered()
{
    impExpManager.importCars(dataB);
    viewCars();
    viewRentals();
}


void MainWindow::on_actionImport_Customers_triggered()
{
    impExpManager.importCustomers(dataB);
    viewCustomers();
    viewRentals();
}


void MainWindow::on_actionImport_Rentals_triggered()
{
    impExpManager.importRentals(dataB);
    viewRentals();
}


void MainWindow::on_actionCreate_templates_triggered()
{
    impExpManager.create_templates();
}


void MainWindow::on_actionExport_cars_triggered()
{
    impExpManager.exportCars(dataB);
}


void MainWindow::on_actionExport_customers_triggered()
{
    impExpManager.exportCustomers(dataB);
}


void MainWindow::on_actionExport_rentals_triggered()
{
    impExpManager.exportRentals(dataB);
}


void MainWindow::on_actionExport_all_triggered()
{
    impExpManager.exportAll(dataB);
}

