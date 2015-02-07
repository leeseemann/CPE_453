#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "trainDisplayInfo.h"

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
    void customLayout(QVector<int>, QVector<int>, QVector<int>);

private:
    Ui::MainWindow *ui;

    QTreeWidgetItem *tracksegment;
    QTreeWidgetItem *trackswitch;
    QTreeWidgetItem *locomotive;
    TrackSegments* new_track;
    TrackSwitches* new_switch;
    Locomotives* new_locomotive;

    void createTopLevelItems();
    void addChildren(QList<TrackSegments*>, QList<TrackSwitches*>, QList<Locomotives*>);

};

#endif // MAINWINDOW_H
