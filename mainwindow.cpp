#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trainDisplayInfo.h"
#include "QDebug"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    /*Initialize and set the graphicsView scene to hold items*/
    QGraphicsScene *scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);

    createTopLevelItems();

    QVector<int> ids;
    ids.push_back(33);
    ids.push_back(34);
    ids.push_back(35);

  //  customLayout(ids, ids, id,  ids);
    sql_information();



}


// this function creates the three top level items in the TreeWidget
void MainWindow::createTopLevelItems()
{
    trackSegmentParentItem = new QTreeWidgetItem;
    trackSegmentParentItem->setText(0, "Track Segments");
    ui->treeWidget->insertTopLevelItem(0, trackSegmentParentItem);
    ui->treeWidget->setColumnWidth(0, 130);


    trackSwitchParentItem = new QTreeWidgetItem;
    trackSwitchParentItem->setText(0, "Track Switches");
    ui->treeWidget->insertTopLevelItem(1, trackSwitchParentItem);
    ui->treeWidget->setColumnWidth(1, 130);

    locomotiveParentItem = new QTreeWidgetItem;
    locomotiveParentItem->setText(0,"Locomotives");
    ui->treeWidget->insertTopLevelItem(2, locomotiveParentItem);
    ui->treeWidget->setColumnWidth(2, 130);


}

// this function populates three QLists that are then used to populate the TreeWidget
void MainWindow::customLayout(QVector<int>&trackSegment_ids, QVector<QString>& trackSegmentStatus, QVector<int>& trackSwitch_ids, QVector<QString>& trackSwitchStatus, QVector<int>&locomotive_ids, QVector<QString>& locomotive_status)
{
    QString segment_label;
    QString segment_number;
    QString switch_label;
    QString switch_number;
    QString locomotive_label;
    QString locomotive_number;

    trackSegmentStatusLabel = new QLabel ("Track Entry Successful. ");
    trackSwitchStatusLabel = new QLabel ("Switch Entry Successful. ");
    locomotiveStatusLabel = new QLabel ("Locomotive Entry Successful. ");

   if(trackSegment_ids.size() == 0)
    {
        trackSegmentStatusLabel->setText("ERROR: Invalid Number of Track Segments ");
    }

    for(int i = 0; i < trackSegment_ids.size(); i++)
    {
        trackSegment = new TrackSegments;

        //set graphicsViewRectItem and position
        trackSegment->setRect(100,100,400, 50);
        ui->graphicsView->scene()->addItem(trackSegment);

        segment_label = "Segment ";
        segment_number = QString::number(i+1);
        segment_label.append(segment_number);
        trackSegment->setTrackSegmentNumber(segment_label);
        trackSegment->setComponentID(trackSegment_ids.at(i));
        trackSegment->setStatus(trackSegmentStatus.at(i));

        if(trackSegmentStatus.at(i) == "Occupied")
        {
            addOccupiedTrack(trackSegment_ids.at(i));
        }

        tracks.insert(i,trackSegment);
    }

    if(trackSwitch_ids.size() == 0)
    {
       trackSwitchStatusLabel->setText("WARNING: No Switches Included in Track Layout ");
    }
    for(int i = 0; i < trackSwitch_ids.size(); i++)
    {

        trackSwitch = new TrackSwitches;

        QGraphicsRectItem *rect = new QGraphicsRectItem;
        rect->setRect(400, 30, 40, 70);
        QColor color;
        color.setRed(75);
        QBrush trackSwitchBackgroundBrush(color);
        rect->setBrush(trackSwitchBackgroundBrush);
        ui->graphicsView->scene()->addItem(rect);

        //set graphicsViewRectItem for Track Switch Status and position
        trackSwitch->setRect(400, 10 ,20, 20);
        trackSwitch->setParentItem(rect);
        ui->graphicsView->scene()->addItem(trackSwitch);

        switch_label = "Switch ";
        switch_number = QString::number(i+1);
        switch_label.append(switch_number);
        trackSwitch->setTrackSwitchNumber(switch_label);
        trackSwitch->setComponentID(trackSwitch_ids.at(i));
        trackSwitch->setStatus(trackSwitchStatus.at(i));

        switches.insert(i,trackSwitch);

    }

   if(locomotive_ids.size() == 0)
    {
        locomotiveStatusLabel->setText("WARNING: No Locomotives Included in Track Layout ");
    }
    for(int i = 0; i < locomotive_ids.size(); i++)
    {
        locomotive = new Locomotives;
        locomotive->setScale(0.2);
        locomotive->setPos(20,35);
        ui->graphicsView->scene()->addItem(locomotive);

        locomotive_label = "Locomotive ";
        locomotive_number = QString::number(i+1);
        locomotive_label.append(locomotive_number);
        locomotive->setLocomotiveNumber(locomotive_label);
        locomotive->setComponentID(locomotive_ids.at(i));
        locomotive->setStatus(locomotive_status.at(i));

        locomotives.insert(i,locomotive);
    }
   statusBar()->addWidget(trackSegmentStatusLabel);
   statusBar()->addWidget(trackSwitchStatusLabel);
   statusBar()->addWidget(locomotiveStatusLabel);

   addChildren(tracks, switches, locomotives);
}

// this function adds all the necessary data to the TreeWidget
void MainWindow::addChildren(QList<TrackSegments*> tracks, QList<TrackSwitches*> switches, QList<Locomotives*> locomotives)
{
    for(int i = 0; i < tracks.size(); i++)
    {
        trackSegmentParentItem->addChild(tracks.at(i));
    }

    for(int j = 0; j < switches.size(); j++)
    {
      trackSwitchParentItem->addChild(switches.at(j));
    }

    for(int k = 0; k < locomotives.size(); k++)
    {
        locomotiveParentItem->addChild(locomotives.at(k));
    }
}


// this function prompts the user for database information
void MainWindow::sql_information()
{
    QDialog* sql_info = new QDialog;
    QGridLayout* layout = new QGridLayout;

    QLabel* title = new QLabel ("SQL Database Information");
    layout->addWidget(title);

    type = new QLineEdit;
    QLabel* type_label = new QLabel("Database Type");
    layout->addWidget(type_label, 1, 0);
    layout->addWidget(type, 1, 1);

    host = new QLineEdit;
    QLabel* host_label = new QLabel("Host");
    layout->addWidget(host_label, 2, 0);
    layout->addWidget(host, 2, 1);

    port = new QLineEdit;
    QLabel* port_label = new QLabel("Port");
    layout->addWidget(port_label, 3,0);
    layout->addWidget(port, 3,1);

    name = new QLineEdit;
    QLabel* name_label = new QLabel("Name");
    layout->addWidget(name_label, 4,0);
    layout->addWidget(name, 4, 1);

    username = new QLineEdit;
    QLabel* username_label = new QLabel("Username");
    layout->addWidget(username_label, 5, 0);
    layout->addWidget(username, 5, 1);

    password = new QLineEdit;
    QLabel* password_label = new QLabel("Password");
    layout->addWidget(password_label, 6, 0);
    layout->addWidget(password, 6, 1);

    QPushButton* default_db = new QPushButton;
    default_db->setText("Use Default Database");
    layout->addWidget(default_db, 7, 0);

    QPushButton* submit = new QPushButton;
    submit->setText("Submit");
    layout->addWidget(submit, 7,1);

    sql_info->setLayout(layout);

   connect(submit, SIGNAL(clicked()), this, SLOT(sql_submit()));
   connect(submit, SIGNAL(clicked()), sql_info, SLOT(close()));
   connect(default_db, SIGNAL(clicked()), this, SLOT(sql_default()));

   sql_info->exec();
}

// this function stores the user's database info and passes it to sql_server
void MainWindow::sql_submit()
{
    db_type = type->text();
    db_host = host->text();
    db_port = port->text();
    db_name = name->text();
    db_username = username->text();
    db_password = password->text();

    sql_connect(db_type, db_host, db_port.toInt(), db_name, db_username, db_password);
}

// this function populates the QDialog with default data if the user chooses to use the default database
void MainWindow::sql_default()
{
    type->setText("QMYSQL");
    host->setText("pavelow");
    port->setText(QString::number(33157));
    name->setText("CPE453");
    username->setText("Team_3B");
    password->setText("Team_3B");
}

// this function creates a connection to the desired SQL Database
void MainWindow::sql_connect(QString db_type, QString db_host, int db_port, QString db_name, QString db_username, QString db_password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase(db_type); // possible issue using QString
    db.setHostName(db_host);
    db.setPort(db_port);
    db.setDatabaseName(db_name);
    db.setUserName(db_username);
    db.setPassword(db_password);

    if(!db.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << db.lastError();
    }
    else
        qDebug() << "Database was opened successfully";

    sql_initialData();

    db.close();

}

// this function retrieves the initial state of the track from the SQL Database and passes the data to customLayout()
void MainWindow::sql_initialData()
{
    // retrieve the initial status and ids from database, pass them to MainWindow::customLayout
    QString q = "SELECT Switch, Position FROM Switches";
    QSqlQuery query;

    if(!query.prepare(q))
        qDebug() << "Query Error: " << query.lastError();
    if(!query.exec())
        qDebug() << "Query Error: " << query.lastError();
    if(!query.isActive())
        qDebug() << "Query Error: " << query.lastError();

    int i = 0;

    while(query.next())
    {
        trackSwitch_ids.push_back(query.value(0).toInt());
        trackSwitchStatus.push_back(query.value(1).toString());

       i++;
    }

    q = "SELECT Train, Current FROM Trains";
    if(!query.prepare(q))
        qDebug() << "Query Error: " << query.lastError();
    if(!query.exec())
        qDebug() << "Query Error: " << query.lastError();
    if(!query.isActive())
        qDebug() << "Query Error: " << query.lastError();


    int j = 0;
    while(query.next())
    {
        locomotive_ids.push_back(query.value(0).toInt());
        locomotiveStatus.push_back(query.value(1).toString());
        j++;
    }


    q = "SELECT Track, Status FROM Tracks";
    if(!query.prepare(q))
        qDebug() << "Query Error: " << query.lastError();
    if(!query.exec())
        qDebug() << "Query Error: " << query.lastError();
    if(!query.isActive())
        qDebug() << "Query Error: " << query.lastError();

    int k = 0;
    while(query.next())
    {
        trackSegment_ids.push_back(query.value(0).toInt());
        trackSegmentStatus.push_back(query.value(1).toString());
        k++;
    }

    customLayout(trackSegment_ids, trackSegmentStatus, trackSwitch_ids, trackSwitchStatus, locomotive_ids, locomotiveStatus);

}

void MainWindow::update_tracks() // update the track status, execute each time the timer expires
{
    // query the database and populate temp_track, compare temp_track status to track_data and update status if necessary
}

void MainWindow::update_switches() // update the switch status, execute each time the timer expires
{
    // query the database and populate temp_switch, compare temp_switch status to switch_data and update status if necessary
}

void MainWindow::update_trains() // update the train status, execute each time the timer expires
{
    // query the database and populate temp_train, compare temp_train status to train_data and update status if necessary
}

void MainWindow::addOccupiedTrack(int id)
{
    QString trackID = QString::number(id);
    ui->occupiedTracksList->addItem(trackID);
}

void MainWindow::clearOccupiedTrack()
{
    ui->occupiedTracksList->clear();
}
MainWindow::~MainWindow()
{
    delete ui;
}
