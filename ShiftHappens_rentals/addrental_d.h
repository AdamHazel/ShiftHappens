#ifndef ADDRENTAL_D_H
#define ADDRENTAL_D_H

#include "mainwindow.h"
#include "shiftdatabase.h"
#include "rentalmanager.h"

#include <QDialog>
#include <QPushButton>

namespace Ui {
class addrental_d;
}

class addrental_d : public QDialog
{
    Q_OBJECT

public:
    explicit addrental_d(QWidget *parent = nullptr);
    ~addrental_d();
    void viewCustomers_rental();
    void viewCars_rental();

protected:
    shiftDatabase dataB;
    QSqlTableModel *customerTable = new QSqlTableModel(this);
    QSqlTableModel *carsTable = new QSqlTableModel(this);
    rentalManager manager;



private slots:
    void on_pushButton_customerSearch_clicked();

    void on_pushButton_carSearch_clicked();

    void on_pushButton_reset_clicked();

    void on_buttonBox_accepted();

    void on_pushButton_checkRental_clicked();

private:
    Ui::addrental_d *ui;
};

#endif // ADDRENTAL_D_H
