#include "assignment_d.h"
#include "ui_assignment_d.h"

assignment_D::assignment_D(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::assignment_D)
{
    ui->setupUi(this);
}

assignment_D::~assignment_D()
{
    delete ui;
}
void assignment_D::newAssignment(){

}
