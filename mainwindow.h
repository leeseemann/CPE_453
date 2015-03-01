#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "trainDisplayInfo.h"
#include <QLabel>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>
#include <QTimer>
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

    QList<TrackSegments*> tracks;
    QList<TrackSwitches*> switches;
    QList<Locomotives*> locomotives;

private:
    Ui::MainWindow *ui;

    userInfo* user_alert;

    QTreeWidgetItem *trackSegmentParentItem;
    QTreeWidgetItem *trackSwitchParentItem;
    QTreeWidgetItem *locomotiveParentItem;
    TrackSegments* trackSegment;
    TrackSwitches* trackSwitch;
    Locomotives* locomotive;
    QVector<int> trackSwitch_ids;
    QVector<QString> trackSwitchStatus;
    QVector<int> trackSegment_ids;
    QVector<QString> trackSegmentStatus;
    QVector<int> locomotive_ids;
    QVector<QString> locomotiveStatus;
    QLabel* trackSegmentStatusLabel;
    QLabel* trackSwitchStatusLabel;
    QLabel* locomotiveStatusLabel;

    QVector<QString> temp_track;
    QVector<QString> temp_switch;
    QVector<QString> temp_train;
    QTimer* status_update;

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


    void addChildren(QList<TrackSegments*>, QList<TrackSwitches*>, QList<Locomotives*>);
    void sql_information();
    void sql_connect(QString db_type, QString db_host, int db_port, QString db_name, QString db_username, QString db_password);
    void sql_initialData();
    void customLayout(QVector<int>&, QVector<QString>&, QVector<int>&, QVector<QString>&, QVector<int>&, QVector<QString>&);
    void createTopLevelItems();
    void addOccupiedTrack(int);
    void clearOccupiedTrack();

private slots:
    void sql_submit();
    void sql_default();
    void update_tracks();
    void update_switches();
    void update_trains();
};

#endif // MAINWINDOW_H
