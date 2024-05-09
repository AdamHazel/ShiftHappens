#include "addcar_d.h"
#include "ui_addcar_d.h"

addCar_D::addCar_D(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addCar_D)
{
    ui->setupUi(this);

    connect(ui->lineEdit_regNr, &QLineEdit::textChanged, this, &addCar_D::validateCar);
    connect(ui->lineEdit_carBrand, &QLineEdit::textChanged, this, &addCar_D::validateCar);
    connect(ui->lineEdit_carModel, &QLineEdit::textChanged, this, &addCar_D::validateCar);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

addCar_D::~addCar_D()
{
    delete ui;
}

void addCar_D::validateCar(){
    bool validated =
        !ui->lineEdit_regNr->text().isEmpty() &&
        !ui->lineEdit_carBrand->text().isEmpty() &&
        !ui->lineEdit_carModel->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(validated);
}

void addCar_D::on_buttonBox_accepted(){
    QString regNr = (ui->lineEdit_regNr->text()).toUpper();
    QString carBrand = (ui->lineEdit_carBrand->text().toUpper());
    QString carModel = (ui->lineEdit_carModel->text().toUpper());
    uint year = (ui->spinBox_year->value());

    dataB.cars_add(&regNr, &carBrand, &carModel, &year);
    QMessageBox::information(this, "Success", "Car with Reg.Nr: " + regNr + " added successfully");
}
