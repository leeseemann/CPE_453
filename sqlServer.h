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
#include "mainwindow.h"
#include "trainDisplayInfo.h"
#include <QSqlQuery>


class sqlServer : public QObject
{
    Q_OBJECT
public:
    explicit sqlServer(QObject *parent = 0);
    QString db_type;
    QString db_host;
    int db_port;
    QString db_name;
    //MainWindow* init_tree;
    QVector<TrackSegments*> track_data;
    QVector<TrackSwitches*> switch_data;
    QVector<Locomotives*> train_data;
    QVector<QString> temp_track;
    QVector<QString> temp_switch;
    QVector<QString> temp_train;

    void sql_connect(QString db_type, QString db_host, int port, QString db_name, QString db_username, QString db_password);
    void sql_initialData(); // retrieve the ids and initial status from the sql database

public slots:
    void update_tracks();
    void update_switches();
    void update_trains();




};

#endif // SQLSERVER_H
