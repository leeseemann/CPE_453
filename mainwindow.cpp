#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "trainDisplayInfo.h"
#include "QDebug"
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QColor>
#include <QTest>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    status_update = new QTimer(this); // create an instance of QTimer
    status_update->setInterval(250);  // set the timer interval

    // when the timer expires, the three update status function will execute
    connect(status_update, SIGNAL(timeout()), this, SLOT(update_trains()));
    connect(status_update, SIGNAL(timeout()), this, SLOT(update_switches()));
    connect(status_update, SIGNAL(timeout()), this, SLOT(update_tracks()));

    //Initialize and set the graphicsView scene to hold items
    QGraphicsScene *scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);


    setupMenu();    //sets up the menu
    setupLegend();  //sets up the colors in the legend
    createTopLevelItems(); // create the top level tree items (Track Segments, Track Switches, Locomotives)

    sql_pavelow(); // allow the user to create a custom connection or connect to pavelow
}


// this function creates the three top level items in the TreeWidget
void MainWindow::createTopLevelItems()
{
    // create the Track Segments tree item and add it to the QTreeWidget
    trackSegmentParentItem = new QTreeWidgetItem;
    trackSegmentParentItem->setText(0, "Track Segments");
    ui->treeWidget->insertTopLevelItem(0, trackSegmentParentItem);
    ui->treeWidget->setColumnWidth(0, 130);

    // create the Track Switches tree item and add it to the QTreeWidget
    trackSwitchParentItem = new QTreeWidgetItem;
    trackSwitchParentItem->setText(0, "Track Switches");
    ui->treeWidget->insertTopLevelItem(1, trackSwitchParentItem);
    ui->treeWidget->setColumnWidth(1, 130);

    // create the Locomotives tree item and add it to the QTreeWidget
    locomotiveParentItem = new QTreeWidgetItem;
    locomotiveParentItem->setText(0,"Locomotives");
    ui->treeWidget->insertTopLevelItem(2, locomotiveParentItem);
    ui->treeWidget->setColumnWidth(2, 130);


}

// this function populates three QLists that are then used to populate the TreeWidget
void MainWindow::customLayout(QVector<QString>&trackSegment_ids, QVector<QString>& trackSegmentStatus, QVector<int>& trackSwitch_ids, QVector<QString>& trackSwitchStatus, QVector<int>&locomotive_ids, QVector<QString>& locomotive_status)
{
    // define necessary variables
    QString segment_label;
    QString segment_number;
    QString switch_label;
    QString switch_number;
    QString locomotive_label;
    QString locomotive_number;

    // initialize QLabel that are placed in the status bar
    trackSegmentStatusLabel = new QLabel ("Track Entry Successful. ");
    trackSwitchStatusLabel = new QLabel ("Switch Entry Successful. ");
    locomotiveStatusLabel = new QLabel ("Locomotive Entry Successful. ");

   // if there are no track segments found, alert the user
   if(trackSegment_ids.size() == 0)
    {
        trackSegmentStatusLabel->setText("ERROR: Invalid Number of Track Segments "); // update status label
        user_alert->message("Invalid Number of Track Segments"); // alert the user
    }


 //CODE FOR PATH INFO**************************************
   QString pathInfo = "SELECT * FROM 'Path Info'";
    QSqlQuery pathQuery(pathInfo, team3b);

       for(int i=0; i<4; i++){
           int k=2;
            pathQuery.next();
            pathArray[i].pathID= pathQuery.value(0).toInt();
            pathArray[i].nextPath= pathQuery.value(1).toInt();
                for(int j=0; j<10; j++) {
                    pathArray[i].next[j] = pathQuery.value(k).toString();
                    k++;
                    }
            if(pathArray[i].nextPath!=0){
                pathArray[pathArray[i].nextPath].pathID==pathArray[i].pathID;

            }
   }//END PATH INFO CODE



   //********CREATES TRACK SEGMENTS IN QTREEWIDGET AND IN QGRAPHICSVIEW

        // this loop queries the database and retrieves all coordinates for each detection section
        for(int i = 0; i < trackSegment_ids.size(); i++)
        {
           //query.next();
           trackSegment = new TrackSegments;

           // create the necessary queries
           QString xcoordQuery ="SELECT Vert_X FROM DS_" + (trackSegment_ids[i]);
           QString ycoordQuery = "SELECT Vert_Y FROM DS_"+ (trackSegment_ids[i]);
           QSqlQuery query1(xcoordQuery, team4b);
           QSqlQuery query2(ycoordQuery, team4b);
                    // while there is coordinate data available, retrieve it
                   while (query1.next() && query2.next() )
                   {
                      QGraphicsRectItem* rect = new QGraphicsRectItem;

                      // set the ToolTip to allow the user to see the section ID during mouse hover
                      QString tooltip = "ID: ";
                      tooltip.append(trackSegment_ids[i]);
                      rect->setToolTip(tooltip);

                      // use query results to plot detection section
                      int xValue = query1.value(0).toInt();
                      xValue = xValue*8.5;
                      int yValue = query2.value(0).toInt();
                      yValue = yValue*8.5;
                      rect->setRect(xValue, yValue, 8.5, 7);
                      trackSegment->addRect(rect);
                      ui->graphicsView->scene()->addItem(rect); // add item to graphicsView


                     //code to set the path in the graphicsView******
                    for(int j=0; j<4; j++){
                        for(int k=0; k<10; k++){
                            if(trackSegment_ids[i] == pathArray[j].next[k]){
                              trackSegment->setPath(pathArray[j].pathID);
                                }
                            }
                        }

                   }//end while loop

                   // create the label that will be displayed in the first column of the Track Segments section in the QTreeWidget
                   segment_label = "Segment ";
                   segment_number = QString::number(i+1);
                   segment_label.append(segment_number);
                   trackSegment->setTrackSegmentNumber(segment_label);

                   // set the ID and status associated with this detection section
                   trackSegment->setComponentID(trackSegment_ids.at(i));
                   trackSegment->setStatus(trackSegmentStatus.at(i));

                   tracks.insert(i,trackSegment); // add this track segment to the master list of track segments
            }

        QSqlQuery query3 ("SELECT Segment, Occupied FROM Occupancy", team3b);

        if(!query3.exec())
        {
            qDebug() << "Query Error: " << query3.lastError();
            pushError(query3.lastError().text()); // add error to error list
        }
        if(!query3.isActive())
        {
            qDebug() << "Query Error: " << query3.lastError();
            pushError(query3.lastError().text()); // add error to error list
        }
        while(query3.next())
        {
            if(query3.value(1) == "1") // a value of 1 indicates an occupied track
            {
                addOccupiedTrack(query3.value(0).toString());
            }
        }
    // if there are no switches found, alert the user
    if(trackSwitch_ids.size() == 0)
    {
       trackSwitchStatusLabel->setText("WARNING: No Switches Included in Track Layout "); // update status label
       user_alert->message("No Switches Included in Track Layout"); // alert the user
    }

   //********CREATES TRACK SWITCHES IN QTREEWIDGET AND IN QGRAPHICSVIEW
    int curSwitch = 0;

    // the following code uses the DIR provided to determine if a segment has a switch and plots the switch if one is present
      for(int j = 0; j < trackSegment_ids.size(); j++)
      {

        // create the necessary queries
        QString xcoordQuery ="SELECT Vert_X FROM DS_" + (trackSegment_ids[j]);
        QString ycoordQuery = "SELECT Vert_Y FROM DS_"+ (trackSegment_ids[j]);
        QString dirQuery = "SELECT DIR FROM DS_"+ (trackSegment_ids[j]);

        QSqlQuery query1(xcoordQuery, team4b);
        QSqlQuery query2(ycoordQuery, team4b);
        QSqlQuery query3(dirQuery, team4b);

              while (query1.next() && query2.next() && query3.next() )
              {
                QString switchCoord=query3.value(0).toString();
                query3.next();
                QString switchCoordNext=query3.value(0).toString();
                int switchX=query1.value(0).toInt();
                int switchY=query2.value(0).toInt();

                // if the DIR values changes, a switch is present
                    if(switchCoord=="A" && switchCoordNext=="B")
                    {
                      switchX = switchX*8.5;
                      switchY = switchY*8.5;

                      // add the switch to the graphics view
                      trackSwitch = new TrackSwitches;
                      trackSwitch->setRect(switchX, switchY, 8.5, 15);
                      ui->graphicsView->scene()->addItem(trackSwitch);

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


  /* for(int k = 0; k < trackSwitch_ids.length(); k++)
   {
       // create the label that will be displayed in the first column of the Track Switches section in the QTreeWidget
       trackSwitch = new TrackSwitches;
       switch_label = "Switch ";
       switch_number = QString::number(k+1);
       switch_label.append(switch_number);
       trackSwitch->setTrackSwitchNumber(switch_label);

       // set the ID and status asssociated with this switch
       trackSwitch->setComponentID(trackSwitch_ids.at(k));
       trackSwitch->setStatus(trackSwitchStatus.at(k));

       switches.insert(curSwitch,trackSwitch);
   }*/


   // if there are no locomotives found, alert the user
   if(locomotive_ids.size() == 0)
    {
        locomotiveStatusLabel->setText("WARNING: No Locomotives Included in Track Layout "); // update status label
        user_alert->message("No Locomotives Included in Track Layout"); // alert the user
    }

   //ADD LOCOMOTIVE GRAPHICS ITEM*************************************
    for(int i = 0; i < locomotive_ids.size(); i++)
    {
       locomotive = new Locomotives;

        // create the label that will be displayed in the first column of the Locomotives section in the QTreeWidget
        locomotive_label = "Locomotive ";
        locomotive_number = QString::number(i+1);
        locomotive_label.append(locomotive_number);
        locomotive->setLocomotiveNumber(locomotive_label);

        // set the ID and status(location) associated with this locomotive
        locomotive->setComponentID(locomotive_ids.at(i));
        locomotive->setStatus(locomotive_status.at(i));

        locomotives.insert(i,locomotive); // add this locomotive to the master list of locomotives
    }

   // update status bar with important information
   statusBar()->addWidget(trackSegmentStatusLabel);
   statusBar()->addWidget(trackSwitchStatusLabel);
   statusBar()->addWidget(locomotiveStatusLabel);

   // call addchildren to populate the QTreeWidget
   addChildren(tracks, switches, locomotives);

   status_update->start(); // start timer
}

// this function adds all the necessary data to the TreeWidget
void MainWindow::addChildren(QList<TrackSegments*> tracks, QList<TrackSwitches*> switches, QList<Locomotives*> locomotives)
{
    // add all the track segments to the QTreeWidget
    for(int i = 0; i < tracks.size(); i++)
    {
        trackSegmentParentItem->addChild(tracks.at(i));
    }

    // add all the track switches to the QTreeWidget
    for(int j = 0; j < switches.size(); j++)
    {
      trackSwitchParentItem->addChild(switches.at(j));
    }

    // add all the locomotives to the QTreeWidget
    for(int k = 0; k < locomotives.size(); k++)
    {
        locomotiveParentItem->addChild(locomotives.at(k));
    }
    for(int l = 0; l < 3; l++)
    {
    ui->treeWidget->resizeColumnToContents(l);
    }
}

void MainWindow::sql_pavelow()
{
    // this dialog allows the user to select the default connection to pavelow or use a custom sql database connection
    QDialog* pavelow = new QDialog;
    QVBoxLayout* layout = new QVBoxLayout;

    QPushButton* use_pavelow = new QPushButton;
    use_pavelow->setText("Connect to Pavelow");

    QPushButton* custom_sql = new QPushButton;
    custom_sql->setText("Create Custom Connection");

    QLabel* title = new QLabel;
    title->setText("Choose an SQL Database Connection");

    // connect the custom_sql button to the dialog that allows the user to enter database info
    connect(custom_sql, SIGNAL(clicked()), this, SLOT(sql_information()));
    connect(custom_sql, SIGNAL(clicked()), pavelow, SLOT(close()));

    // connect the use_pavelow button to the function which will create the needed connections to pavelow automatically
    connect(use_pavelow, SIGNAL(clicked()), this, SLOT(connect_pavelow()));
    connect(use_pavelow, SIGNAL(clicked()), pavelow, SLOT(close()));

    layout->addWidget(title);
    layout->addWidget(use_pavelow);
    layout->addWidget(custom_sql);

    pavelow->setLayout(layout);
    pavelow->exec();
}

void MainWindow::connect_pavelow()
{
    // create connections to the individual sql databases that are needed to populate the display
    team4b = QSqlDatabase::addDatabase("QMYSQL", "4b");
    team4b.setHostName("pavelow.eng.uah.edu");
    team4b.setPort(33158);
    team4b.setDatabaseName("team4b");
    team4b.setUserName("team3b");
    team4b.setPassword("ulimbese");

    if(!team4b.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << team4b.lastError();
        user_alert->message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }

    team3b = QSqlDatabase::addDatabase("QMYSQL", "3b");
    team3b.setHostName("pavelow.eng.uah.edu");
    team3b.setPort(33157);
    team3b.setDatabaseName("fqt_test");
    team3b.setUserName("team3b");
    team3b.setPassword("ulimbese");

    if(!team3b.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << team3b.lastError();
        user_alert->message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }
}

// this function prompts the user for database information
void MainWindow::sql_information()
{
    QDialog* sql_info = new QDialog;
    QGridLayout* layout = new QGridLayout;

    QLabel* title = new QLabel ("SQL Database Information");
    layout->addWidget(title);

    // create line edit and add it to the layout
    type = new QLineEdit;
    QLabel* type_label = new QLabel("Database Type");
    layout->addWidget(type_label, 1, 0);
    layout->addWidget(type, 1, 1);

    // create line edit and add it to the layout
    host = new QLineEdit;
    QLabel* host_label = new QLabel("Host");
    layout->addWidget(host_label, 2, 0);
    layout->addWidget(host, 2, 1);

    // create line edit and add it to the layout
    port = new QLineEdit;
    QLabel* port_label = new QLabel("Port");
    layout->addWidget(port_label, 3,0);
    layout->addWidget(port, 3,1);

    // create line edit and add it to the layout
    name = new QLineEdit;
    QLabel* name_label = new QLabel("Name");
    layout->addWidget(name_label, 4,0);
    layout->addWidget(name, 4, 1);

    // create line edit and add it to the layout
    username = new QLineEdit;
    QLabel* username_label = new QLabel("Username");
    layout->addWidget(username_label, 5, 0);
    layout->addWidget(username, 5, 1);

    // create line edit and add it to the layout
    password = new QLineEdit;
    QLabel* password_label = new QLabel("Password");
    layout->addWidget(password_label, 6, 0);
    layout->addWidget(password, 6, 1);

    // create QPushButton and add it to the layout
    QPushButton* default_db = new QPushButton;
    default_db->setText("Use Default Database");
    layout->addWidget(default_db, 7, 0);

    // create QPushButton and add it to the layout
    QPushButton* submit = new QPushButton;
    submit->setText("Submit");
    layout->addWidget(submit, 7,1);

    sql_info->setLayout(layout);

   connect(submit, SIGNAL(clicked()), this, SLOT(sql_submit())); // user submits data
   connect(submit, SIGNAL(clicked()), sql_info, SLOT(close()));  // close the dialog when user submits data
   connect(default_db, SIGNAL(clicked()), this, SLOT(sql_default())); // user selects default database information

   sql_info->exec();

}

// this function stores the user's database info and passes it to sql_server
void MainWindow::sql_submit()
{
    db_type = type->text().toUpper();
    db_host = host->text();
    db_port = port->text();
    db_name = name->text();
    db_username = username->text();
    db_password = password->text();

    sql_connect(db_type, db_host, db_port.toInt(), db_name, db_username, db_password); // use the information to connect to the desired database
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
    // create a QSqlDatabase and set login data
    team3b = QSqlDatabase::addDatabase(db_type, "4b");
    team3b.setHostName(db_host);
    team3b.setPort(db_port);
    team3b.setDatabaseName(db_name);
    team3b.setUserName(db_username);
    team3b.setPassword(db_password);

    team4b = QSqlDatabase::addDatabase(db_type, "3b");
    team4b.setHostName(db_host);
    team4b.setPort(db_port);
    team4b.setDatabaseName(db_name);
    team4b.setUserName(db_username);
    team4b.setPassword(db_password);

    // verify that database was opened successfully
    // if failure occurs, alert the user and exit the program
    if(!team3b.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << team3b.lastError();
        user_alert->message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }

    else
    {
        qDebug() << "Database was opened successfully"; // if database opens successfully
    }

    // verify that database was opened successfully
    // if failure occurs, alert the user and exit the program
    if(!team4b.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << team4b.lastError();
        user_alert->message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }

    else
    {
        qDebug() << "Database was opened successfully"; // if database opens successfully
    }

    sql_initialData(); // retrieve desired data from the database

   // db.close(); // close the database connection

}

// this function retrieves the initial state of the track from the SQL Database and passes the data to customLayout()
void MainWindow::sql_initialData()
{
    // retrieve the initial status and ids from database, pass them to MainWindow::customLayout
    QSqlQuery query ("SELECT Switch, Position FROM Switches", team3b);
    bool badDB = false;

    // verify that the query is valid and executes properly
    if(!query.exec())
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text()); // add error to error list
        badDB = true; // 'true' indicates failure
    }
    if(!query.isActive())
    {
        qDebug() << "Query Error: " << query.lastError();
        pushError(query.lastError().text()); // add error to error list
        badDB = true; // 'true' indicates failure
    }

    int i = 0;

    while(query.next()) // while there is data to retrieve
    {
        trackSwitch_ids.push_back(query.value(0).toInt()); // store the id of the switch
        trackSwitchStatus.push_back(query.value(1).toString()); // store the current status of the switch

       i++;
    }

    QSqlQuery query2("SELECT Train, Current FROM Trains", team3b); // retrieve train id and current location

    // verify that the query is valid and executes properly

    if(!query2.exec())
    {
        qDebug() << "Query Error: " << query2.lastError();
        pushError(query2.lastError().text()); // add error to error list
        badDB = true; // 'true' indicates failure
    }
    if(!query2.isActive())
    {
        qDebug() << "Query Error: " << query2.lastError();
        pushError(query2.lastError().text()); // add error to error list
        badDB = true; // 'true' indicates failure
    }


    int j = 0;
    while(query2.next()) // while there is data to retrieve
    {
        locomotive_ids.push_back(query2.value(0).toInt()); // store train id
        locomotiveStatus.push_back(query2.value(1).toString()); // store the current location of the train
        j++;
    }


    QSqlQuery query3("SELECT Track, Status FROM Tracks", team3b); // retrieve track segment ids and the current status

    // verify that the query is valid and executes properly
    if(!query3.exec())
    {
        qDebug() << "Query Error: " << query3.lastError();
        pushError(query3.lastError().text()); // add error to error list
        badDB = true; // 'true' indicates failure
    }
    if(!query3.isActive())
    {
        qDebug() << "Query Error: " << query3.lastError();
        pushError(query3.lastError().text()); // add error to error list
        badDB = true; // 'true' indicates failure
    }

    int k = 0;
    while(query3.next()) // while there is data to retrieve
    {
        trackSegment_ids.push_back(query3.value(0).toString()); // store track segment id
        trackSegmentStatus.push_back(query3.value(1).toString()); // store track segment's current status
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

    // populate the tree with the data retrieved
    customLayout(trackSegment_ids, trackSegmentStatus, trackSwitch_ids, trackSwitchStatus, locomotive_ids, locomotiveStatus);

}

void MainWindow::update_tracks() // update the track status, execute each time the timer expires
{
    QSqlQuery query_trackStatus("SELECT Status FROM Tracks", team3b);

    // verify query is valid and executes properly
    if(!query_trackStatus.exec())
    {
        qDebug() << "Query Error: " << query_trackStatus.lastError();
        user_alert->message(query_trackStatus.lastError()); // alert user
    }
    if(!query_trackStatus.isActive())
    {
        qDebug() << "Query Error: " << query_trackStatus.lastError();
        user_alert->message(query_trackStatus.lastError()); // alert user
    }


    while(query_trackStatus.next())
        temp_track.push_back(query_trackStatus.value(0).toString()); // store current track status in temporary vector

    // if the new status is different than the current status, update the status
    for(int k = 0; k < temp_track.length(); k++)
    {
        if(temp_track.at(k) != tracks.at(k)->getStatus())
            tracks.at(k)->setStatus(temp_track.at(k));
    }
    temp_track.clear(); // clear temp vector to prepare for next update

    // check to see which tracks are now occcupied
   QSqlQuery query_trackStatus2("SELECT Segment, Occupied FROM Occupancy", team3b);

    // verify query is valid and executes properly
    if(!query_trackStatus2.exec())
    {
        qDebug() << "Query Error: " << query_trackStatus2.lastError();
        user_alert->message(query_trackStatus2.lastError()); // alert user
    }
    if(!query_trackStatus2.isActive())
    {
        qDebug() << "Query Error: " << query_trackStatus2.lastError();
        user_alert->message(query_trackStatus2.lastError()); // alert user
    }


    while(query_trackStatus2.next())
    {
        // if a track segment is occupied, update the status of that track segment
        if(query_trackStatus2.value(1).toString() == "TRUE")
        {
            for(int j = 0; j < tracks.length(); j++)
            {
                if(tracks.at(j)->getComponentID() == query_trackStatus2.value(0).toString())
                    tracks.at(j)->setStatus("Occupied");
            }
        }
    }

    clearOccupiedTrack(); // clear the list of occupied tracks to prepare for new list of occupied tracks

    for(int i = 0; i < tracks.length(); i++)
    {
        // if a track is occupied, add it to the display
        if(tracks.at(i)->getStatus() == "Occupied")
        {
            addOccupiedTrack(tracks.at(i)->getComponentID());
        }
    }

}

void MainWindow::update_switches() // update the switch status, execute each time the timer expires
{
    QSqlQuery query_switchStatus("SELECT Position FROM Switches", team3b);

    // verify query is valid and executes properly
    if(!query_switchStatus.exec())
    {
        qDebug() << "Query Error: " << query_switchStatus.lastError();
        user_alert->message(query_switchStatus.lastError()); // alert user
    }

    if(!query_switchStatus.isActive())
    {
        qDebug() << "Query Error: " << query_switchStatus.lastError();
        user_alert->message(query_switchStatus.lastError()); // alert user
    }


    while(query_switchStatus.next())
        temp_switch.push_back(query_switchStatus.value(0).toString()); // store current status in a temporary vector

    for(int k = 0; k < temp_switch.length(); k++)
    {
        // if new status is different than current status, update the display
        if(temp_switch.at(k) != switches.at(k)->getStatus())
            switches.at(k)->setStatus(temp_switch.at(k));
    }

    temp_switch.clear(); // clear temporary vector to prepare for next
}

void MainWindow::update_trains() // update the train status, execute each time the timer expires
{
    QSqlQuery query_trainStatus("SELECT Current FROM Trains", team3b);

    // verify query is valid and executes properly
    if(!query_trainStatus.exec())
    {
        qDebug() << "Query Error: " << query_trainStatus.lastError();
        user_alert->message(query_trainStatus.lastError()); // alert user
    }
    if(!query_trainStatus.isActive())
    {
        qDebug() << "Query Error: " << query_trainStatus.lastError();
        user_alert->message(query_trainStatus.lastError()); // alert user
    }

    while(query_trainStatus.next())
        temp_train.push_back(query_trainStatus.value(0).toString()); // store current train location in a temporary vector

    for(int k = 0; k < temp_train.length(); k++)
    {
        // if new location is different than current location, update the location
        if(temp_train.at(k) != locomotives.at(k)->getStatus())
            locomotives.at(k)->setStatus(temp_train.at(k));
    }

    temp_train.clear(); // clear temporary vector to prepare for next update

}

//Eric's Functions for stuff
void MainWindow::addOccupiedTrack(QString id)
{
    ui->occupiedTracksList->addItem(id); // add track with the desired id to the list of currently occupied tracks
}

void MainWindow::clearOccupiedTrack()
{
    ui->occupiedTracksList->clear(); // clear list of currently occupied tracks
    if(ui->occupiedTracksList->currentItem() != NULL)
    {
        qDebug() << "SYSTEMFAIL";
    }
}

void MainWindow::pushError(QString Err)
{
    errorStack.push_back(Err); // add error to vector of errors
}

void MainWindow::printErrors()
{
    QString temp;
    // display list of errors
    while(!errorStack.empty())
    {
        temp = errorStack.takeFirst();
        qDebug() << temp;
    }
}

void MainWindow::setupLegend()
{
    /*Set the colors and sizing for the legend*/
    QColor color;
    color.setGreen(255);
    ui->legend->setItem(0,0, new QTableWidgetItem);
    ui->legend->item(0,0)->setBackgroundColor(color);

    color.setRgb(255,0,0);
    ui->legend->setItem(1,0, new QTableWidgetItem);
    ui->legend->item(1,0)->setData(Qt::BackgroundRole,color);

    color.setRgb(255,255,0);
    ui->legend->setItem(2,0, new QTableWidgetItem);
    ui->legend->item(2,0)->setData(Qt::BackgroundRole,color);

    color.setRgb(79,80,82);
    ui->legend->setItem(3,0, new QTableWidgetItem);
    ui->legend->item(3,0)->setData(Qt::BackgroundRole,color);

    color.setRgb(200,140,237);
    ui->legend->setItem(4,0, new QTableWidgetItem);
    ui->legend->item(4,0)->setData(Qt::BackgroundRole,color);

    //Set the sizing for rows
    QHeaderView *verticalHeader = ui->legend->verticalHeader();
    verticalHeader->sectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(18);
    //Disable ability to select items
    ui->legend->setSelectionMode(QAbstractItemView::NoSelection);
}

void MainWindow::setupMenu()
{
    testAct = new QAction(("&Test Program"),this);
    menu = menuBar()->addMenu("&Menu");
    menu->addAction(testAct);
    connect(testAct,SIGNAL(triggered()),this,SLOT(test_program()));
}

void MainWindow::test_program()
{
    QString test = "testID";
    addOccupiedTrack(test);
    if(test.compare(ui->occupiedTracksList->item(0)->text()) != 0)
    {
        user_alert->message("Add Occupied Track Test has Failed");
        goto bail;
    }
    clearOccupiedTrack();
    if(ui->occupiedTracksList->count() != NULL)
    {
        user_alert->message("Clear Occupied Track Test has Failed");
        goto bail;
    }


bail:
    return;
}
MainWindow::~MainWindow()
{
    delete ui;
}
