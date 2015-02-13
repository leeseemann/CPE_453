#ifndef SQLSERVER_H
#define SQLSERVER_H

#include <QObject>
#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QtSql>
#include <QSqlDatabase>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QObject>
#include <QCheckBox>

class sqlServer : public QObject
{
    Q_OBJECT
public:
    explicit sqlServer(QObject *parent = 0);
    void sql_init();
    void sql_connect(QString db_type, QString db_host, int port, QString db_name, QString db_username, QString db_password);
    QString db_type;
    QString db_host;
    int db_port;
    QString db_name;

};

#endif // SQLSERVER_H
