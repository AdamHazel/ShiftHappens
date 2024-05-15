/**
 * @class
 * /

#ifndef ASSIGNMENT_D_H
#define ASSIGNMENT_D_H

#include "mainwindow.h"
#include "shiftdatabase.h"

#include <QDialog>
#include <QPushButton>

namespace Ui {
class assignment_D;
}

class assignment_D : public QDialog
{
    Q_OBJECT

public:
    explicit assignment_D(QWidget *parent = nullptr);
    ~assignment_D();

protected:
    shiftDatabase dataB;

private:
    Ui::assignment_D *ui;
private slots:
    void newAssignment();
};

#endif // ASSIGNMENT_D_H
