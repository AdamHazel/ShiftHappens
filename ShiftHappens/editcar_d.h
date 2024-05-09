#ifndef EDITCAR_D_H
#define EDITCAR_D_H

#include "mainwindow.h"
#include "shiftdatabase.h"

#include <QDialog>
#include <QPushButton>

namespace Ui {
class editCar_D;
}

class editCar_D: public QDialog
{
    Q_OBJECT
public:
    explicit editCar_D(QString& regNr, QWidget *parent = nullptr);
    ~editCar_D();
protected:
    shiftDatabase dataB;

private:
    Ui::editCar_D *ui;
private slots:
    void on_buttonBox_accepted();
    void validateCar();
    void fetchCar(QString& regNr);
};

#endif // EDITCAR_D_H
