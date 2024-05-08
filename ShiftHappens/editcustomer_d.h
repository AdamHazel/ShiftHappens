#ifndef EDITCUSTOMER_D_H
#define EDITCUSTOMER_D_H

#include "mainwindow.h"
#include "shiftdatabase.h"

#include <QDialog>
#include <QPushButton>

namespace Ui {
class editCustomer_D;
}

class editCustomer_D : public QDialog
{
    Q_OBJECT
public:
    explicit editCustomer_D(uint& id, QWidget *parent = nullptr);
    ~editCustomer_D();
private:
    Ui::editCustomer_D *ui;
private slots:
    void on_buttonBox_accepted();
    void validateCustomer();
    void fetchCustomer(uint& id);
};

#endif // EDITCUSTOMER_D_H
