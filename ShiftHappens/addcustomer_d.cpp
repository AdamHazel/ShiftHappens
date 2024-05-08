#include "addcustomer_d.h"
#include "ui_addcustomer_d.h"

addCustomer_D::addCustomer_D(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addCustomer_D)
{
    ui->setupUi(this);

    connect(ui->lineEdit_custName, &QLineEdit::textChanged, this, &addCustomer_D::validateCustomer);
    connect(ui->lineEdit_street, &QLineEdit::textChanged, this, &addCustomer_D::validateCustomer);
    connect(ui->lineEdit_city, &QLineEdit::textChanged, this, &addCustomer_D::validateCustomer);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

addCustomer_D::~addCustomer_D()
{
    delete ui;
}

void addCustomer_D::validateCustomer(){
    bool validated =
    !ui->lineEdit_custName->text().isEmpty() &&
    !ui->lineEdit_street->text().isEmpty() &&
    !ui->lineEdit_city->text().isEmpty();
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(validated);
}

void addCustomer_D::on_buttonBox_accepted(){
    QString custName = (ui->lineEdit_custName->text()).toUpper();
    QString custStreet = (ui->lineEdit_street->text().toUpper());
    uint postNum = (ui->spinBox_PCode->value());
    QString custCity = (ui->lineEdit_city->text().toUpper());

    qDebug() << custName << " " << custStreet <<" " << postNum << " " << custCity;

    shiftDatabase db;
    db.customer_add(&custName, &custStreet, &postNum, &custCity);
    QMessageBox::information(this, "Success!", "Customer " + custName + " added successfully");
}
