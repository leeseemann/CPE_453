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

    QTreeWidgetItem *tracksegment;
    QTreeWidgetItem *trackswitch;
    QTreeWidgetItem *locomotive;
    TrackSegments* new_track;
    TrackSwitches* new_switch;
    Locomotives* new_locomotive;
    QVector<int> switch_ids;
    QVector<QString> switch_status;
    QVector<int> track_ids;
    QVector<QString> track_status;
    QVector<int> train_ids;
    QVector<QString> train_status;
    QLabel* statusTrack;
    QLabel* statusSwitch;
    QLabel* statusLocomotive;

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
    void update_tracks();
    void update_switches();
    void update_trains();
    void customLayout(QVector<int>&, QVector<QString>&, QVector<int>&, QVector<QString>&, QVector<int>&, QVector<QString>&);
    void createTopLevelItems();


private slots:
    void sql_submit();
    void sql_default();


};

#endif // MAINWINDOW_H
