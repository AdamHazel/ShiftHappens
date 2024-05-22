/**
 * @class addCustomer_D (D for dialog) object handles user interaction when user wishes to add a customer
 */

#ifndef ADDCUSTOMER_D_H
#define ADDCUSTOMER_D_H

#include "mainwindow.h"
#include "shiftdatabase.h"

#include <QDialog>
#include <QPushButton>

namespace Ui {
class addCustomer_D;
}

class addCustomer_D : public QDialog
{
    Q_OBJECT

public:
    explicit addCustomer_D(QWidget *parent = nullptr);
    ~addCustomer_D();

protected:
    shiftDatabase dataB;

private slots:
    void on_buttonBox_accepted();
    void validateCustomer();

private:
    Ui::addCustomer_D *ui;
};

#endif // ADDCUSTOMER_D_H
