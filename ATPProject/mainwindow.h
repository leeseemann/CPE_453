#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>
#include "userinfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    userInfo* user_alert; // instance of userInfo class

    // parts of dialog used to allow user to enter info for connecting to an sql database
    QLineEdit* type;
    QLineEdit* host;
    QLineEdit* port;
    QLineEdit* name;
    QLineEdit* username;
    QLineEdit* password;

    // used to store user-entered sql database info
    QString db_type;
    QString db_host;
    QString db_port;
    QString db_name;
    QString db_username;
    QString db_password;

    QSqlDatabase team3b;
    QSqlDatabase team4b;

    // functions
    void sql_connect(QString db_type, QString db_host, int db_port, QString db_name, QString db_username, QString db_password);
    void sql_initialData();
    void sql_pavelow();

private slots:
    void sql_submit();
    void sql_default();
    void sql_information();
    void connect_pavelow();
    void acceptance_test();
};

#endif // MAINWINDOW_H
