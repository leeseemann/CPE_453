#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trainDisplayInfo.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    createTopLevelItems();

    QVector<int> ids;
    ids.push_back(33);
    ids.push_back(34);
    ids.push_back(35);

    customLayout(ids, ids, ids);

   /* //TEST DATA
    TrackSegments *seg1 = new TrackSegments();
    seg1->setTrackSegmentNumber("Segment 1");
    seg1->setComponentID(35);
    seg1->setStatus("Active");

    TrackSegments *seg2 = new TrackSegments();
    seg2->setTrackSegmentNumber("Segment 2");
    seg2->setComponentID(36);
    seg2->setStatus("Occupied");

    TrackSegments *seg3 = new TrackSegments();
    seg3->setTrackSegmentNumber("Segment 3");
    seg3->setComponentID(37);
    seg3->setStatus("Inactive");

    TrackSwitches *switch1 = new TrackSwitches();
    switch1->setTrackSwitchNumber("Switch 1");
    switch1->setComponentID(21);
    switch1->setStatus("Bypass");

    TrackSwitches *switch2 = new TrackSwitches();
    switch2->setTrackSwitchNumber("Switch 2");
    switch2->setComponentID(22);
    switch2->setStatus("Through");

    Locomotives *loco1 = new Locomotives();
    loco1->setLocomotiveNumber("Locomotive 1");
    loco1->setComponentID(88);
    loco1->setStatus("On Segment 33");

    Locomotives *loco2 = new Locomotives();
    loco2->setLocomotiveNumber("Locomotive 2");
    loco2->setComponentID(89);
    loco2->setStatus("On Segment 37");

   QTreeWidgetItem *tracksegment = new QTreeWidgetItem();
   tracksegment->setText(0, "Track Segments");
   tracksegment->addChild(seg1);
     tracksegment->addChild(seg2);
       tracksegment->addChild(seg3);

   ui->treeWidget->insertTopLevelItem(0,tracksegment);


   QTreeWidgetItem *trackswitch = new QTreeWidgetItem();
   trackswitch->setText(0, "Track Switches");
   trackswitch->addChild(switch1);
    trackswitch->addChild(switch2);

   ui->treeWidget->insertTopLevelItem(1,trackswitch);

    QTreeWidgetItem *locomotive = new QTreeWidgetItem();
    locomotive->setText(0,"Locomotives");
    locomotive->addChild(loco1);
      locomotive->addChild(loco2);

    ui->treeWidget->insertTopLevelItem(2,locomotive );*/

   // tracksegment->addChild(track.tracks.at(0));

}


// this function creates the three top level items in the TreeWidget
void MainWindow::createTopLevelItems()
{
    tracksegment = new QTreeWidgetItem;
    tracksegment->setText(0, "Track Segments");
    ui->treeWidget->insertTopLevelItem(0, tracksegment);

    trackswitch = new QTreeWidgetItem;
    trackswitch->setText(0, "Track Switches");
    ui->treeWidget->insertTopLevelItem(1, trackswitch);

    locomotive = new QTreeWidgetItem;
    locomotive->setText(0,"Locomotives");
    ui->treeWidget->insertTopLevelItem(2, locomotive);

}

// this function populates three QLists that are then used to populate the TreeWidget
void MainWindow::customLayout(QVector<int>track_ids, QVector<int>switch_ids, QVector<int>locomotive_ids)
{
    for(int i = 0; i < track_ids.size(); i++)
    {
        new_track = new TrackSegments;
        QString segment = "Segment ";
        QString segment_number = QString::number(i+1);
        segment.append(segment_number);
        new_track->setTrackSegmentNumber(segment);
        new_track->setComponentID(track_ids.at(i));
        new_track->setStatus("Inactive");

        tracks.insert(i,new_track);
    }

    for(int i = 0; i < switch_ids.size(); i++)
    {
        new_switch = new TrackSwitches;
        QString switch_label = "Switch ";
        QString switch_number = QString::number(i+1);
        switch_label.append(switch_number);
        new_switch->setTrackSwitchNumber(switch_label);
        new_switch->setComponentID(switch_ids.at(i));
        new_switch->setStatus("Bypass");

        switches.insert(i,new_switch);
    }

    for(int i = 0; i < locomotive_ids.size(); i++)
    {
        new_locomotive = new Locomotives;
        QString locomotive_label = "Locomotive ";
        QString locomotive_number = QString::number(i+1);
        locomotive_label.append(locomotive_number);
        new_locomotive->setLocomotiveNumber(locomotive_label);
        new_locomotive->setComponentID(locomotive_ids.at(i));
        new_locomotive->setStatus("");

        locomotives.insert(i,new_locomotive);
    }
    addChildren(tracks, switches, locomotives);
}

// this function adds all the necessary data to the TreeWidget
void MainWindow::addChildren(QList<TrackSegments*> tracks, QList<TrackSwitches*> switches, QList<Locomotives*> locomotives)
{
    for(int i = 0; i < tracks.size(); i++)
    {
        tracksegment->addChild(tracks.at(i));
    }

    for(int j = 0; j < switches.size(); j++)
    {
        trackswitch->addChild(switches.at(j));
    }

    for(int k = 0; k < locomotives.size(); k++)
    {
        locomotive->addChild(locomotives.at(k));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
