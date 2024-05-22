/**
 * @class addCar_D (D for dialog) object handles user interaction when user wishes to add a car
 */

#ifndef ADDCAR_D_H
#define ADDCAR_D_H

#include "mainwindow.h"
#include "shiftdatabase.h"

#include <QDialog>
#include <QPushButton>

namespace Ui {
class addCar_D;
}

class addCar_D : public QDialog
{
    Q_OBJECT

public:
    explicit addCar_D(QWidget *parent = nullptr);
    ~addCar_D();

protected:
    shiftDatabase dataB;

private slots:
    void on_buttonBox_accepted();
    void validateCar();

private:
    Ui::addCar_D *ui;
};

#endif // ADDCAR_D_H
