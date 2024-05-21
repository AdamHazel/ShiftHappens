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

/**
 * @brief Validates if all line edits are filled
 */
void addCar_D::validateCar(){
    bool validated =
        !ui->lineEdit_regNr->text().isEmpty() &&
        !ui->lineEdit_carBrand->text().isEmpty() &&
        !ui->lineEdit_carModel->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(validated);
}

/**
 * @brief Adds car to the database (with error checks)
 */
void addCar_D::on_buttonBox_accepted(){
    QString regNr = (ui->lineEdit_regNr->text()).toUpper();
    QString carBrand = (ui->lineEdit_carBrand->text().toUpper());
    QString carModel = (ui->lineEdit_carModel->text().toUpper());
    uint dayPrice = (ui->spinBox_dayPrice->value());

    bool confirmed = false;
    confirmed = dataB.cars_add(&regNr, &carBrand, &carModel, &dayPrice);
    if (confirmed == true)
        QMessageBox::information(this, "Success", "Car with Reg.Nr: " + regNr + " added successfully");
    else
    {
        QMessageBox errorMessage;
        errorMessage.setWindowTitle("Error");
        errorMessage.setText("Car with Reg.Nr" + regNr + "not added");
        errorMessage.setDetailedText("Check that the car does not already exist");
        errorMessage.exec();
    }
}

