#ifndef ASSIGNMENT_D_H
#define ASSIGNMENT_D_H

#include "mainwindow.h"
#include "shiftdatabase.h"

#include <QDialog>
#include <QPushButton>

namespace Ui {
class assignment;
}

class assignment : public QDialog
{
    Q_OBJECT

public:
    explicit assignment(QWidget *parent = nullptr);
    ~assignment();

private:
    Ui::assignment *ui;
private slots:
    void newAssignment();
};

#endif // ASSIGNMENT_D_H
