#include "addcustomer_d.h"
#include "ui_addcustomer_d.h"

/**
 * @brief Constructor for addCustomer_D
 * @param QWidget parent
 */
addCustomer_D::addCustomer_D(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addCustomer_D)
{
    ui->setupUi(this);
}

/**
 * @brief Deconstructor
 */
addCustomer_D::~addCustomer_D()
{
    delete ui;
}

/**
 * @brief Validates if all line edits are filled
 */
void addCustomer_D::validateCustomer(){
    bool validated =
        !ui->lineEdit_custName->text().isEmpty() &&
        !ui->lineEdit_street->text().isEmpty() &&
        !ui->lineEdit_city->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(validated);
}

/**
 * @brief Adds customer to database when "ok" is pressed (produces error message if unsuccesful)
 */
void addCustomer_D::on_buttonBox_accepted()
{
    QString custName = (ui->lineEdit_custName->text()).toUpper();
    QString custStreet = (ui->lineEdit_street->text().toUpper());
    uint postNum = (ui->spinBox_PCode->value());
    QString custCity = (ui->lineEdit_city->text().toUpper());

    qDebug() << custName << " " << custStreet <<" " << postNum << " " << custCity;

    bool confirmed = false;
    confirmed = dataB.customer_add(&custName, &custStreet, &postNum, &custCity);
    if (confirmed == true)
        QMessageBox::information(this, "Success!", "Customer " + custName + " added successfully");
    else
    {
        QMessageBox errorMessage;
        errorMessage.setWindowTitle("Error");
        errorMessage.setText("Customer " + custName + "not added");
        errorMessage.setDetailedText("Check that the customer does not already exist");
        errorMessage.exec();
    }
}

