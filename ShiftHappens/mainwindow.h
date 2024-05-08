#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QMessageBox>
#include <QAction>

#include "shiftdatabase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void viewCustomers();
    void viewCars();
protected:
    shiftDatabase dataB;
    static const uint regNrColumn = 0;
    static const uint IdColumn = 0;
private slots:
    void on_addCarB_clicked();
    void on_editCarB_clicked();
    void on_removeCarB_clicked();
    void on_addCustB_clicked();
    void on_editCustB_clicked();
    void on_removeCustB_clicked();
    void on_newAssignB_clicked();
    void on_actionExit_triggered();
    void on_actionImport_customers_triggered();

    void on_actionImport_cars_triggered();

    void on_actionCreate_templates_triggered();

    void on_actionExport_customers_triggered();

    void on_actionExport_cars_triggered();

    void on_actionExport_all_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
