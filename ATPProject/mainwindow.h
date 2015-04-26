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

    QSqlDatabase fqt_test;
    QSqlDatabase team4b;

    // functions
    void sql_connect(QString db_type, QString db_host, int db_port, QString db_name, QString db_username, QString db_password);
    void sql_initialData();
    void sql_pavelow();
    void message(QString);
    void message(QSqlError);
    void message(QString,QVector<QString>);

private slots:
    void sql_submit();
    void sql_default();
    void sql_information();
    void connect_pavelow();
    void acceptance_test();
    // overloaded functions for displaying a message to the user

};

#endif // MAINWINDOW_H
