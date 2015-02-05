#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trainDisplayInfo.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    //TEST DATA
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

    ui->treeWidget->insertTopLevelItem(2,locomotive );

}

MainWindow::~MainWindow()
{
    delete ui;
}
