#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addcar_d.h"
#include "addcustomer_d.h"
#include "editcar_d.h"
#include "editcustomer_d.h"

#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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

        shiftDatabase db;
        if (db.cars_removeByRegNr(&RegNr)) {
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
        shiftDatabase db;
        if (db.customer_removeByID(&customerId)) {
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
    ui->tableView_cars->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView_cars->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_cars->resizeColumnsToContents();
    ui->tableView_cars->show();
}

void MainWindow::on_actionExit_triggered()
{
    if
        (QMessageBox::Yes == QMessageBox::question(this, "Confirmation", "Are you sure you want to close the program?", QMessageBox::Yes | QMessageBox::No))
        std::exit(0);
    else
        return;
}

