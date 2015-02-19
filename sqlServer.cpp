#include "sqlServer.h"
#include <QtSql>

sqlServer::sqlServer(QObject *parent) :
    QObject(parent)
{
}

void sqlServer::sql_connect(QString db_type, QString db_host, int db_port, QString db_name, QString db_username, QString db_password)
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
        qDebug() << db.lastError().text();
    }
    else
        qDebug() << "Database was opened successfully";\

    sql_initialData();

    db.close();

}

void sqlServer::sql_initialData()
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
     //  switch_data.at(i)->setComponentID(query.value(0).toInt()); // this will probably need to be a string based on what i've seen from other teams
     //  switch_data.at(i)->setStatus(query.value(1).toString());
       i++;
       qDebug() << "ID: " << query.value(0);
       qDebug() << "Status: " << query.value(1);
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
     //   train_data.at(j)->setComponentID(query.value(0).toInt());
     //   train_data.at(j)->setStatus(query.value(1).toString());
        j++;
        qDebug() << "ID: " << query.value(0);
        qDebug() << "Status: " << query.value(1);
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
     //   track_data.at(k)->setComponentID(query.value(0).toInt());
     //   track_data.at(k)->setStatus(query.value(1).toString());
        k++;
        qDebug() << "ID: " << query.value(0);
        qDebug() << "Status: " << query.value(1);
    }

}

void sqlServer::update_tracks() // update the track status, execute each time the timer expires
{
    // query the database and populate temp_track, compare temp_track status to track_data and update status if necessary
}

void sqlServer::update_switches() // update the switch status, execute each time the timer expires
{
    // query the database and populate temp_switch, compare temp_switch status to switch_data and update status if necessary
}

void sqlServer::update_trains() // update the train status, execute each time the timer expires
{
    // query the database and populate temp_train, compare temp_train status to train_data and update status if necessary
}






