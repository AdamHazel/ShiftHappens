#include "editcar_d.h"
#include "ui_editcar_d.h"

editCar_D::editCar_D(QString& regNr, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editCar_D)
{
    ui->setupUi(this);

    connect(ui->lineEdit_newBrand, &QLineEdit::textChanged, this, &editCar_D::validateCar);
    connect(ui->lineEdit_newModel, &QLineEdit::textChanged, this, &editCar_D::validateCar);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    fetchCar(regNr);
}

editCar_D::~editCar_D()
{
    delete ui;
}
void editCar_D::validateCar(){
    bool validated =
        !ui->lineEdit_newBrand->text().isEmpty() &&
        !ui->lineEdit_newModel->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(validated);
}
void editCar_D::fetchCar(QString& regNr){
    shiftDatabase db;
    QString brand, model;
    uint year;

    (db.cars_fetchById(regNr, brand, model, year));
        ui->lineEdit_oldRegNr->setText(regNr);
        ui->lineEdit_newRegNr->setText(regNr);
        ui->lineEdit_oldBrand->setText(brand);
        ui->lineEdit_oldModel->setText(model);
        ui->lineEdit_oldYear->setText(QString::number(year));
}
void editCar_D::on_buttonBox_accepted(){
    QString regNr = (ui->lineEdit_newRegNr->text()).toUpper();
    QString carBrand = (ui->lineEdit_newBrand->text().toUpper());
    QString carModel = (ui->lineEdit_newModel->text().toUpper());
    uint year = (ui->spinBox_newYear->value());

    shiftDatabase db;
    db.cars_updateByRegNr(&regNr, &carBrand, &carModel, &year);
    QMessageBox::information(this, "Success", "Car edited successfully!");
}
