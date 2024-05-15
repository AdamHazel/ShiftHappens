#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcar_d.h"
#include "addcustomer_d.h"
#include "editcar_d.h"
#include "editcustomer_d.h"
#include "assignment_d.h"

#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dataB.cars_createTable();
    dataB.customer_createTable();
    viewCars();
    viewCustomers();
    viewRentals();
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
void MainWindow::on_newAssignB_clicked()
{
    assignment_D dialog(this);
    dialog.exec();
}


void MainWindow::on_remAssignB_clicked()
{

}

void MainWindow::viewCustomers(){
    QSqlTableModel *customerTable = new QSqlTableModel;
    customerTable->setTable("customers");
    customerTable->setHeaderData(0, Qt::Horizontal, tr("Customer ID"));
    customerTable->setHeaderData(1, Qt::Horizontal, tr("Name"));
    customerTable->setHeaderData(2, Qt::Horizontal, tr("Street"));
    customerTable->setHeaderData(3, Qt::Horizontal, tr("Postcode"));
    customerTable->setHeaderData(4, Qt::Horizontal, tr("City"));
    customerTable->select();

    ui->tableView_customers->setModel(customerTable);
    ui->tableView_customers->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_customers->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_customers->resizeColumnsToContents();
    ui->tableView_customers->show();
}
void MainWindow::viewCars(){
    QSqlTableModel *carsTable = new QSqlTableModel;
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
    QSqlTableModel *rentalsTable = new QSqlTableModel;
    rentalsTable->setTable("cars");
    rentalsTable->setFilter("custID = NULL");
    rentalsTable->select();
    rentalsTable->setHeaderData(0, Qt::Horizontal, tr("Reg. Num"));
    rentalsTable->setHeaderData(4, Qt::Horizontal, tr("Assigned to"));

    ui->tableView_rentals->setModel(rentalsTable);
    ui->tableView_rentals->setColumnHidden(1, true);
    ui->tableView_rentals->setColumnHidden(2, true);
    ui->tableView_rentals->setColumnHidden(3, true);
    ui->tableView_rentals->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_rentals->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_rentals->resizeColumnsToContents();
    ui->tableView_rentals->show();
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
    importManager.importCustomers(dataB);
    viewCustomers();
}


void MainWindow::on_actionImport_cars_triggered()
{
    importManager.importCars(dataB);
    viewCars();
}


void MainWindow::on_actionCreate_templates_triggered()
{
    importManager.create_templates();
}


void MainWindow::on_actionExport_customers_triggered()
{
    exportManager.exportCustomers(dataB);
}


void MainWindow::on_actionExport_cars_triggered()
{
    exportManager.exportCars(dataB);
}


void MainWindow::on_actionExport_all_triggered()
{
    exportManager.exportAll(dataB);
}
