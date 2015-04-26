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
#include <QMenu>
#include <QMenuBar>
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

    QList<TrackSegments*> tracks; // master list of tracks
    QList<TrackSwitches*> switches; // master list of switches
    QList<Locomotives*> locomotives; // master list of trains

private:
    Ui::MainWindow *ui;

    userInfo* user_alert; // instance of userInfo class

    // parents of the QTreeWidget
    QTreeWidgetItem *trackSegmentParentItem;
    QTreeWidgetItem *trackSwitchParentItem;
    QTreeWidgetItem *locomotiveParentItem;

    TrackSegments* trackSegment;
    TrackSwitches* trackSwitch;
    Locomotives* locomotive;

    // vectors to store switch, track, and train ids and statuses
    QVector<int> trackSwitch_ids;
    QVector<QString> trackSwitchStatus;
    QVector<QString> trackSegment_ids;
    QVector<QString> trackSegmentStatus;
    QVector<int> locomotive_ids;
    QVector<QString> locomotiveStatus;

   // structure to store path info
    struct pathStruct{
        int pathID;
        int nextPath;
        QString next[11];
    }pathArray[4]; //4 path max?

    // status labels
    QLabel* trackSegmentStatusLabel;
    QLabel* trackSwitchStatusLabel;
    QLabel* locomotiveStatusLabel;

    // temporary vector used for updating status of components
    QVector<QString> temp_track;
    QVector<QString> temp_switch;
    QVector<QString> temp_train;

    // timer
    QTimer* status_update;

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
    QMenu *menu;
    QAction *testAct;


    // functions
    void addChildren(QList<TrackSegments*>, QList<TrackSwitches*>, QList<Locomotives*>);
    void sql_connect(QString db_type, QString db_host, int db_port, QString db_name, QString db_username, QString db_password);
    void sql_initialData();
    void customLayout(QVector<QString>&, QVector<QString>&, QVector<int>&, QVector<QString>&, QVector<int>&, QVector<QString>&);
    void createTopLevelItems();
    void setupLegend();
    void setupMenu();
    void addOccupiedTrack(QString);
    void clearOccupiedTrack();
    void sql_pavelow();

    //These are for the error stack
    void pushError(QString);
    void printErrors();
    QVector<QString> errorStack;



private slots:
    void sql_submit();
    void sql_default();
    void update_tracks();
    void update_switches();
    void update_trains();
    void sql_information();
    void connect_pavelow();
    void test_program();
};

#endif // MAINWINDOW_H
