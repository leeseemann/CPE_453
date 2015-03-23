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
    status_update = new QTimer(this);
    status_update->setInterval(250);

    connect(status_update, SIGNAL(timeout()), this, SLOT(update_trains()));
    connect(status_update, SIGNAL(timeout()), this, SLOT(update_switches()));
    connect(status_update, SIGNAL(timeout()), this, SLOT(update_tracks()));

    /*Initialize and set the graphicsView scene to hold items*/
    QGraphicsScene *scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);

    createTopLevelItems();

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
void MainWindow::customLayout(QVector<QString>&trackSegment_ids, QVector<QString>& trackSegmentStatus, QVector<int>& trackSwitch_ids, QVector<QString>& trackSwitchStatus, QVector<int>&locomotive_ids, QVector<QString>& locomotive_status)
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
        user_alert->message("Invalid Number of Track Segments");
    }




   //********CREATES TRACK SEGMENTS IN QTREEWIDGET AND IN QGRAPHICSVIEW
  // QSqlQuery query("SELECT Track FROM Tracks");

        for(int i = 0; i < trackSegment_ids.size(); i++)
        {
           //query.next();
           trackSegment = new TrackSegments;

           QString xcoordQuery ="SELECT Vert_X FROM DS_" + (trackSegment_ids[i]);
           QString ycoordQuery = "SELECT Vert_Y FROM DS_"+ (trackSegment_ids[i]);
            QSqlQuery query1(xcoordQuery);
            QSqlQuery query2(ycoordQuery);

                   while (query1.next() && query2.next())
                   {
                      QGraphicsRectItem* rect = new QGraphicsRectItem;
                      int xValue = query1.value(0).toInt();
                      xValue = xValue*7;
                      int yValue = query2.value(0).toInt();
                      yValue = yValue*7;
                      rect->setRect(xValue, yValue, 7, 5);
                      trackSegment->addRect(rect);
                      ui->graphicsView->scene()->addItem(rect);
                   }

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
       user_alert->message("No Switches Included in Track Layout");
    }

   //********CREATES TRACK SWITCHES IN QTREEWIDGET AND IN QGRAPHICSVIEW
    int curSwitch = 0;

      for(int j = 0; j < trackSegment_ids.size(); j++)
      {

        QString xcoordQuery ="SELECT Vert_X FROM DS_" + (trackSegment_ids[j]);
        QString ycoordQuery = "SELECT Vert_Y FROM DS_"+ (trackSegment_ids[j]);
        QString dirQuery = "SELECT DIR FROM DS_"+ (trackSegment_ids[j]);

        QSqlQuery query1(xcoordQuery);
        QSqlQuery query2(ycoordQuery);
        QSqlQuery query3(dirQuery);

              while (query1.next() && query2.next() && query3.next() )
              {
                QString switchCoord=query3.value(0).toString();
                query3.next();
                QString switchCoordNext=query3.value(0).toString();
                int switchX=query1.value(0).toInt();
                int switchY=query2.value(0).toInt();

                    if(switchCoord=="A" && switchCoordNext=="B")
                    {
                      switchX = switchX*7;
                      switchY = switchY*7;

                      trackSwitch = new TrackSwitches;
                      trackSwitch->setRect(switchX, switchY, 7, 12);
                      ui->graphicsView->scene()->addItem(trackSwitch);

                      // initialize QTreeWidget switches

                      switch_label = "Switch ";
                      switch_number = QString::number(curSwitch+1);
                      switch_label.append(switch_number);
                      trackSwitch->setTrackSwitchNumber(switch_label);
                      trackSwitch->setComponentID(trackSwitch_ids.at(curSwitch));
                      trackSwitch->setStatus(trackSwitchStatus.at(curSwitch));

                      switches.insert(curSwitch,trackSwitch);
                      ++curSwitch;
                    }
                    query3.previous();
              }
        }


   if(locomotive_ids.size() == 0)
    {
        locomotiveStatusLabel->setText("WARNING: No Locomotives Included in Track Layout ");
        user_alert->message("No Locomotives Included in Track Layout");
    }

   //ADD LOCOMOTIVE GRAPHICS ITEM*************************************
    for(int i = 0; i < locomotive_ids.size(); i++)
    {
       locomotive = new Locomotives;

        // initialize the QTreeWidget trains
        locomotive_label = "Locomotive ";
        locomotive_number = QString::number(i+1);
        locomotive_label.append(locomotive_number);
        locomotive->setLocomotiveNumber(locomotive_label);
        locomotive->setComponentID(locomotive_ids.at(i));
        locomotive->setStatus(locomotive_status.at(i));

        locomotives.insert(i,locomotive);
    }

   // update status bar with important information
   statusBar()->addWidget(trackSegmentStatusLabel);
   statusBar()->addWidget(trackSwitchStatusLabel);
   statusBar()->addWidget(locomotiveStatusLabel);

   addChildren(tracks, switches, locomotives);

   status_update->start(); // start timer
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
        user_alert->message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }
    else
    {
        qDebug() << "Database was opened successfully";
    }

    sql_initialData();

   // db.close(); // close the database connection

}

// this function retrieves the initial state of the track from the SQL Database and passes the data to customLayout()
void MainWindow::sql_initialData()
{
    // retrieve the initial status and ids from database, pass them to MainWindow::customLayout
    QString q = "SELECT Switch, Position FROM Switches";
    QSqlQuery query;
    bool badDB = false;

    if(!query.prepare(q))
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text());
        badDB = true;
    }
    if(!query.exec())
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text());
        badDB = true;
    }
    if(!query.isActive())
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text());
        badDB = true;
    }

    int i = 0;

    while(query.next())
    {
        trackSwitch_ids.push_back(query.value(0).toInt());
        trackSwitchStatus.push_back(query.value(1).toString());

       i++;
    }

    q = "SELECT Train, Current FROM Trains";
    if(!query.prepare(q))
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text());
        badDB = true;
    }
    if(!query.exec())
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text());
        badDB = true;
    }
    if(!query.isActive())
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text());
        badDB = true;
    }


    int j = 0;
    while(query.next())
    {
        locomotive_ids.push_back(query.value(0).toInt());
        locomotiveStatus.push_back(query.value(1).toString());
        j++;
    }


    q = "SELECT Track, Status FROM Tracks";
    if(!query.prepare(q))
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text());
        badDB = true;
    }
    if(!query.exec())
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text());
        badDB = true;
    }
    if(!query.isActive())
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text());
        badDB = true;
    }

    int k = 0;
    while(query.next())
    {
        trackSegment_ids.push_back(query.value(0).toString());
        trackSegmentStatus.push_back(query.value(1).toString());
        k++;
    }

    //If any of the querys fail then the database is not set up right.
    //Generate the message and close everything.
    if(badDB == true)
    {
        user_alert->message("The database is bad and it should feel bad.\n Closing the application!",errorStack);
        printErrors();  //pring errors to debug too
        exit(1);
    }

    customLayout(trackSegment_ids, trackSegmentStatus, trackSwitch_ids, trackSwitchStatus, locomotive_ids, locomotiveStatus);

}

void MainWindow::update_tracks() // update the track status, execute each time the timer expires
{
    QSqlQuery query_trackStatus;
    QString trackStatus_update;
    trackStatus_update = "SELECT Status FROM Tracks";

    if(!query_trackStatus.prepare(trackStatus_update))
    {
        qDebug() << "Query Error: " << query_trackStatus.lastError();
        user_alert->message(query_trackStatus.lastError());
    }
    if(!query_trackStatus.exec())
    {
        qDebug() << "Query Error: " << query_trackStatus.lastError();
        user_alert->message(query_trackStatus.lastError());
    }
    if(!query_trackStatus.isActive())
    {
        qDebug() << "Query Error: " << query_trackStatus.lastError();
        user_alert->message(query_trackStatus.lastError());
    }


    while(query_trackStatus.next())
        temp_track.push_back(query_trackStatus.value(0).toString());

    for(int k = 0; k < temp_track.length(); k++)
    {
        if(temp_track.at(k) != tracks.at(k)->getStatus())
            tracks.at(k)->setStatus(temp_track.at(k));
    }
    temp_track.clear();

    trackStatus_update = "SELECT Segment, Occupied FROM Occupancy";
    if(!query_trackStatus.prepare(trackStatus_update))
    {
        qDebug() << "Query Error: " << query_trackStatus.lastError();
        user_alert->message(query_trackStatus.lastError());
    }
    if(!query_trackStatus.exec())
    {
        qDebug() << "Query Error: " << query_trackStatus.lastError();
        user_alert->message(query_trackStatus.lastError());
    }
    if(!query_trackStatus.isActive())
    {
        qDebug() << "Query Error: " << query_trackStatus.lastError();
        user_alert->message(query_trackStatus.lastError());
    }


    while(query_trackStatus.next())
    {
        if(query_trackStatus.value(1).toString() == "TRUE")
        {
            for(int j = 0; j < tracks.length(); j++)
            {
                if(tracks.at(j)->getComponentID() == query_trackStatus.value(0).toString())
                    tracks.at(j)->setStatus("Occupied");
            }
        }
    }

    clearOccupiedTrack();

    for(int i = 0; i < tracks.length(); i++)
    {
        if(tracks.at(i)->getStatus() == "Occupied")
        {
            addOccupiedTrack(tracks.at(i)->getComponentID());
        }
    }

}

void MainWindow::update_switches() // update the switch status, execute each time the timer expires
{
    QSqlQuery query_switchStatus;
    QString switchStatus_update;
    switchStatus_update = "SELECT Position FROM Switches";

    if(!query_switchStatus.prepare(switchStatus_update))
    {
        qDebug() << "Query Error: " << query_switchStatus.lastError();
        user_alert->message(query_switchStatus.lastError());
    }
    if(!query_switchStatus.exec())
    {
        qDebug() << "Query Error: " << query_switchStatus.lastError();
        user_alert->message(query_switchStatus.lastError());
    }

    if(!query_switchStatus.isActive())
    {
        qDebug() << "Query Error: " << query_switchStatus.lastError();
        user_alert->message(query_switchStatus.lastError());
    }


    while(query_switchStatus.next())
        temp_switch.push_back(query_switchStatus.value(0).toString());

    for(int k = 0; k < temp_switch.length(); k++)
    {
        if(temp_switch.at(k) != switches.at(k)->getStatus())
            switches.at(k)->setStatus(temp_switch.at(k));
    }

    temp_switch.clear();
}

void MainWindow::update_trains() // update the train status, execute each time the timer expires
{
    QSqlQuery query_trainStatus;
    QString trainStatus_update;
    trainStatus_update = "SELECT Current FROM Trains";

    if(!query_trainStatus.prepare(trainStatus_update))
    {
        qDebug() << "Query Error: " << query_trainStatus.lastError();
        user_alert->message(query_trainStatus.lastError());
    }
    if(!query_trainStatus.exec())
    {
        qDebug() << "Query Error: " << query_trainStatus.lastError();
        user_alert->message(query_trainStatus.lastError());
    }
    if(!query_trainStatus.isActive())
    {
        qDebug() << "Query Error: " << query_trainStatus.lastError();
        user_alert->message(query_trainStatus.lastError());
    }

    while(query_trainStatus.next())
        temp_train.push_back(query_trainStatus.value(0).toString());

    for(int k = 0; k < temp_train.length(); k++)
    {
        if(temp_train.at(k) != locomotives.at(k)->getStatus())
            locomotives.at(k)->setStatus(temp_train.at(k));
    }

    temp_train.clear();

}

//Eric's Functions for stuff
void MainWindow::addOccupiedTrack(QString id)
{
    ui->occupiedTracksList->addItem(id);
}

void MainWindow::clearOccupiedTrack()
{
    ui->occupiedTracksList->clear();
}

void MainWindow::pushError(QString Err)
{
    errorStack.push_back(Err);
}

void MainWindow::printErrors()
{
    QString temp;
    while(!errorStack.empty())
    {
        temp = errorStack.takeFirst();
        qDebug() << temp;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
