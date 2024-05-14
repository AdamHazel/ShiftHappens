#include "assignment_d.h"
#include "ui_assignment_d.h"

assignment::assignment(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::assignment)
{
    ui->setupUi(this);
}

assignment::~assignment()
{
    delete ui;
}
void assignment::newAssignment(){}
