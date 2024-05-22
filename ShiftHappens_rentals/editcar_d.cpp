#include "editcar_d.h"
#include "ui_editcar_d.h"
#include "car.h"

/**
 * @brief Constructor for editCar_D
 * @param QWidget parent
 */
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

/**
 * @brief Deconstructor
 */
editCar_D::~editCar_D()
{
    delete ui;
}

/**
 * @brief Validates if all line edits are filled
 */
void editCar_D::validateCar(){
    bool validated =
        !ui->lineEdit_newBrand->text().isEmpty() &&
        !ui->lineEdit_newModel->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(validated);
}

/**
 * @brief Fetches car and places fetched values in line edits
 * @param QString&      reference to regNr that will be fetched
 */
void editCar_D::fetchCar(QString& regNr){
    car temp = dataB.cars_fetchCar(regNr);

    ui->lineEdit_oldRegNr->setText(regNr);
    ui->lineEdit_newRegNr->setText(regNr);
    ui->lineEdit_oldBrand->setText(temp.brand);
    ui->lineEdit_oldModel->setText(temp.model);
    ui->lineEdit_oldDayPrice->setText(QString::number(temp.dayPrice));
}

/**
 * @brief Updates car to the database (with error checks)
 */
void editCar_D::on_buttonBox_accepted()
{
    QString regNr = (ui->lineEdit_newRegNr->text()).toUpper();
    QString carBrand = (ui->lineEdit_newBrand->text().toUpper());
    QString carModel = (ui->lineEdit_newModel->text().toUpper());
    uint dayPrice = (ui->spinBox_DayPrice->value());

    bool confirmed = false;
    confirmed = dataB.cars_updateByRegNr(&regNr, &carBrand, &carModel, &dayPrice);
    if (confirmed == true)
        QMessageBox::information(this, "Success", "Car edited successfully!");
    else
    {
        QMessageBox errorMessage;
        errorMessage.setWindowTitle("Error");
        errorMessage.setText("Car with Reg.Nr" + regNr + "not edited");
        errorMessage.exec();
    }
}

