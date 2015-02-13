#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "trainDisplayInfo.h"
#include "sqlServer.h"
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QList<TrackSegments*> tracks;
    QList<TrackSwitches*> switches;
    QList<Locomotives*> locomotives;
    void customLayout(QVector<int>&, QVector<int>&, QVector<int>&);


private:
    Ui::MainWindow *ui;

    QTreeWidgetItem *tracksegment;
    QTreeWidgetItem *trackswitch;
    QTreeWidgetItem *locomotive;
    TrackSegments* new_track;
    TrackSwitches* new_switch;
    Locomotives* new_locomotive;
    QLabel* track_status;
    QLabel* switch_status;
    QLabel* locomotive_status;

    QLineEdit* type;
    QLineEdit* host;
    QLineEdit* port;
    QLineEdit* name;
    QLineEdit* username;
    QLineEdit* password;
    QString db_type;
    QString db_host;
    QString db_port;
    QString db_name;
    QString db_username;
    QString db_password;
    sqlServer* sql;

    void createTopLevelItems();
    void addChildren(QList<TrackSegments*>, QList<TrackSwitches*>, QList<Locomotives*>);
    void sql_information();

private slots:
    void sql_submit();
    void sql_default();


};

#endif // MAINWINDOW_H
