/**
 * @class Main window class. Window that opens when program is started
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "shiftdatabase.h"

#include <QMainWindow>
#include <QTableView>
#include <QMessageBox>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    shiftDatabase dataB;
    static const uint regNrColumn = 0;
    static const uint IdColumn = 0;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void viewCustomers();
    void viewCars();
    void viewRentals();
    void viewAll();

private slots:
    void on_addCarB_clicked();

    void on_editCarB_clicked();

    void on_removeCarB_clicked();

    void on_addCustB_clicked();

    void on_editCustB_clicked();

    void on_removeCustB_clicked();

    void on_addRentalB_clicked();

    void on_removeRentalB_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
