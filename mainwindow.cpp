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
    seg2->setStatus("Lauren");

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
    switch2->setStatus("Dog");

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

    ui->treeWidget->insertTopLevelItem(2,locomotive );

   // tracksegment->addChild(track.tracks.at(0));*/

}


// this function creates the three top level items in the TreeWidget
void MainWindow::createTopLevelItems()
{
    tracksegment = new QTreeWidgetItem;
    tracksegment->setText(0, "Track Segments");
    ui->treeWidget->insertTopLevelItem(0, tracksegment);
    ui->treeWidget->setColumnWidth(0, 130);


    trackswitch = new QTreeWidgetItem;
    trackswitch->setText(0, "Track Switches");
    ui->treeWidget->insertTopLevelItem(1, trackswitch);
    ui->treeWidget->setColumnWidth(1, 130);

    locomotive = new QTreeWidgetItem;
    locomotive->setText(0,"Locomotives");
    ui->treeWidget->insertTopLevelItem(2, locomotive);
    ui->treeWidget->setColumnWidth(2, 130);


}

// this function populates three QLists that are then used to populate the TreeWidget
void MainWindow::customLayout(QVector<int>&track_ids, QVector<int>&switch_ids, QVector<int>&locomotive_ids)
{
    QString segment_label;
    QString segment_number;
    QString switch_label;
    QString switch_number;
    QString locomotive_label;
    QString locomotive_number;
    track_status = new QLabel ("Track Entry Successful. ");
    switch_status = new QLabel ("Switch Entry Successful. ");
    locomotive_status = new QLabel ("Locomotive Entry Successful. ");

    if(track_ids.size() == 0)
    {
        track_status->setText("ERROR: Invalid Number of Track Segments ");
    }

    for(int i = 0; i < track_ids.size(); i++)
    {
        new_track = new TrackSegments;
        segment_label = "Segment ";
        segment_number = QString::number(i+1);
        segment_label.append(segment_number);
        new_track->setTrackSegmentNumber(segment_label);
        new_track->setComponentID(track_ids.at(i));
        new_track->setStatus("Inactive");

        tracks.insert(i,new_track);
    }

    if(switch_ids.size() == 0)
    {
        switch_status->setText("WARNING: No Switches Included in Track Layout ");
    }
    for(int i = 0; i < switch_ids.size(); i++)
    {
        new_switch = new TrackSwitches;
        switch_label = "Switch ";
        switch_number = QString::number(i+1);
        switch_label.append(switch_number);
        new_switch->setTrackSwitchNumber(switch_label);
        new_switch->setComponentID(switch_ids.at(i));
        new_switch->setStatus("Bypass");

        switches.insert(i,new_switch);
    }

    if(locomotive_ids.size() == 0)
    {
        locomotive_status->setText("WARNING: No Locomotives Included in Track Layout ");
    }
    for(int i = 0; i < locomotive_ids.size(); i++)
    {
        new_locomotive = new Locomotives;
        locomotive_label = "Locomotive ";
        locomotive_number = QString::number(i+1);
        locomotive_label.append(locomotive_number);
        new_locomotive->setLocomotiveNumber(locomotive_label);
        new_locomotive->setComponentID(locomotive_ids.at(i));
        new_locomotive->setStatus("");

        locomotives.insert(i,new_locomotive);
    }
    statusBar()->addWidget(track_status);
    statusBar()->addWidget(switch_status);
    statusBar()->addWidget(locomotive_status);
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
