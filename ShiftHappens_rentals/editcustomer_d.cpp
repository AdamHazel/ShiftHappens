#include "editcustomer_d.h"
#include "ui_editcustomer_d.h"
#include "customer.h"

editCustomer_D::editCustomer_D(uint& id, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::editCustomer_D)
{
    ui->setupUi(this);

    connect(ui->lineEdit_newName, &QLineEdit::textChanged, this, &editCustomer_D::validateCustomer);
    connect(ui->lineEdit_newStreet, &QLineEdit::textChanged, this, &editCustomer_D::validateCustomer);
    connect(ui->lineEdit_newCity, &QLineEdit::textChanged, this, &editCustomer_D::validateCustomer);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    fetchCustomer(id);
}

editCustomer_D::~editCustomer_D()
{
    delete ui;
}

/**
 * @brief Validates if all line edits are filled
 */
void editCustomer_D::validateCustomer(){
    bool validated =
        !ui->lineEdit_newName->text().isEmpty() &&
        !ui->lineEdit_newStreet->text().isEmpty() &&
        !ui->lineEdit_newCity->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(validated);
}

/**
 * @brief Fetches car and places fetched values in line edits
 * @param uint&      reference to customer id that will be fetched
 */
void editCustomer_D::fetchCustomer(uint& id){

    customer temp = dataB.customer_fetchCustomer(id);

    ui->lineEdit_oldId->setText(QString::number(id));
    ui->lineEdit_newId->setText(QString::number(id));
    ui->lineEdit_oldName->setText(temp.name);
    ui->lineEdit_oldStreet->setText(temp.street);
    ui->lineEdit_oldPCode->setText(QString::number(temp.postcode));
    ui->lineEdit_oldCity->setText(temp.city);
}

void editCustomer_D::on_buttonBox_accepted()
{
    uint id = ui->lineEdit_newId->text().toUInt();
    QString name = (ui->lineEdit_newName->text()).toUpper();
    QString street = (ui->lineEdit_newStreet->text().toUpper());
    uint pCode = (ui->spinBox_newPCode->value());
    QString city = (ui->lineEdit_newCity->text().toUpper());

    bool confirmed = false;
    confirmed = dataB.customer_updateByID(&id, &name, &street, &pCode, &city);
    if (confirmed == true)
        QMessageBox::information(this, "Success", "Customer edited successfully!");
    else
    {
        QMessageBox errorMessage;
        errorMessage.setWindowTitle("Error");
        errorMessage.setText("Customer " + name + "not edited");
        errorMessage.exec();
    }
}

