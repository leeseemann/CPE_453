#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include <QListWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sql_pavelow(); // allow the user to create a custom connection or connect to pavelow
    connect(ui->btnTest,SIGNAL(clicked()),this,SLOT(acceptance_test()));
    connect(ui->btnTest, SIGNAL(clicked()), this, SLOT(next_test()));

    init_database();

}

MainWindow::~MainWindow()
{
    QString queryPrep2 = "DROP TABLE track_ds";
    QSqlQuery query299 (queryPrep2, fqt_test);
    if(!query299.exec())
        qDebug() << "Query Error: " << query299.lastError();

    queryPrep2 = "DROP TABLE track_power";
    QSqlQuery query300 (queryPrep2, fqt_test);
    if(!query300.exec())
        qDebug() << "Query Error: " << query300.lastError();

    queryPrep2 = "DROP TABLE track_switch_5a";
    QSqlQuery query301 (queryPrep2, fqt_test);
    if(!query301.exec())
        qDebug() << "Query Error: " << query301.lastError();

    queryPrep2 = "DROP TABLE scheduled_train_info";
    QSqlQuery query302 (queryPrep2, fqt_test);
    if(!query302.exec())
        qDebug() << "Query Error: " << query302.lastError();

    queryPrep2 = "DROP TABLE scheduled_routes";
    QSqlQuery query303 (queryPrep2, fqt_test);
    if(!query303.exec())
        qDebug() << "Query Error: " << query303.lastError();

    team4b.close();
    fqt_test.close();

    delete ui;
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

  //  connect(ui->btnTest, )

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
        message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }

    fqt_test = QSqlDatabase::addDatabase("QMYSQL", "3b");
    fqt_test.setHostName("pavelow.eng.uah.edu");
    fqt_test.setPort(33157);
    fqt_test.setDatabaseName("fqt_test");
    fqt_test.setUserName("team3b");
    fqt_test.setPassword("ulimbese");

    if(!fqt_test.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << fqt_test.lastError();
        message("Database was not opened successfully.\n Closing the application!");
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
    QString path_to_database = "C:/Users/Eric/Documents/GitHub/build-CPE_453-Desktop_Qt_5_3_MSVC2013_64bit-Debug"+db_name;
    // create a QSqlDatabase and set login data
    fqt_test = QSqlDatabase::addDatabase(db_type, "4b");
    fqt_test.setHostName(db_host);
    fqt_test.setPort(db_port);
    fqt_test.setDatabaseName(path_to_database);
    fqt_test.setUserName(db_username);
    fqt_test.setPassword(db_password);

    team4b = QSqlDatabase::addDatabase(db_type, "3b");
    team4b.setHostName(db_host);
    team4b.setPort(db_port);
    team4b.setDatabaseName(path_to_database);
    team4b.setUserName(db_username);
    team4b.setPassword(db_password);

    // verify that database was opened successfully
    // if failure occurs, alert the user and exit the program
    if(!fqt_test.open())
    {
        qDebug() << "Database was not opened successfully";
        qDebug() << fqt_test.lastError();
        message("Database was not opened successfully.\n Closing the application!");
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
        message("Database was not opened successfully.\n Closing the application!");
        exit(2);
    }

    else
    {
        qDebug() << "Database was opened successfully"; // if database opens successfully
    }

   // db.close(); // close the database connection

}

//This function is where changes are made to the database and the user verifies the changes in the display software
void MainWindow::acceptance_test()
{

    QSqlQuery query("UPDATE Occupancy SET Occupied = 'TRUE' WHERE Segment = '2_7'", fqt_test);

    bool badDB = false;

    // verify that the query is valid and executes properly
    if(!query.exec())
    {
        qDebug() << "Query Error: " << query.lastError();
        //pushError(query.lastError().text()); // add error to error list
        badDB = true; // 'true' indicates failure
    }
    if(!query.isActive())
    {
        qDebug() << "Query Error: " << query.lastError();
        //pushError(query.lastError().text()); // add error to error list
        badDB = true; // 'true' indicates failure
    }
}

void MainWindow::message(QString alert)
{
    QDialog* user_alert = new QDialog;
    QVBoxLayout* layout = new QVBoxLayout;
    //QString Err;
    //int ErrStackSize = ErrorStack.getStackSize();

    QLabel* title = new QLabel("Acceptance Tests");
    title->setAlignment(Qt::AlignHCenter);
    title->setFont(QFont("Helvetica", 16, QFont::Bold));

    QLabel* message = new QLabel(alert);
    message->setAlignment(Qt::AlignHCenter);
    message->setFont(QFont("Helvetica", 12));

    QPushButton* ok = new QPushButton;
    ok->setText("OK");
   connect(ok, SIGNAL(clicked()), user_alert, SLOT(close()));


    //QListWidget* errors = new QListWidget();

    //for(int iter = 0; iter < ErrStackSize; iter++)
    //{
    //    Err = ErrorStack.getStack();
    //    errors->addItem();
    //}

    layout->addWidget(title);
    layout->addSpacing(15);
    layout->addWidget(message);
    layout->addSpacing(25);
    layout->addWidget(ok);
    //layout->addWidget(errors);
    //layout->addSpacing(25);

  //  layout->addWidget(accept);

    user_alert->setLayout(layout);

    user_alert->exec();
}

 void MainWindow::init_database()
 {

     QString queryPrep = "CREATE TABLE track_ds (id TEXT, status BOOLEAN, ds_from TEXT, ds_to TEXT)";
     QSqlQuery query (queryPrep, fqt_test);
     if(!query.exec())
         qDebug() << "Query Error: " << query.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?, ?, ?, ?)";
     QSqlQuery query2 (queryPrep, fqt_test);
     query2.addBindValue("1_1");
     query2.addBindValue("FALSE");
     query2.addBindValue("NULL");
     query2.addBindValue("NULL");
     if(!query2.exec())
         qDebug() << "Query Error: " << query2.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query3 (queryPrep, fqt_test);
     query3.addBindValue("1_10");
     query3.addBindValue("FALSE");
     query3.addBindValue("NULL");
     query3.addBindValue("NULL");
     if(!query3.exec())
         qDebug() << "Query Error: " << query3.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query4 (queryPrep, fqt_test);
     query4.addBindValue("1_11");
     query4.addBindValue("FALSE");
     query4.addBindValue("NULL");
     query4.addBindValue("NULL");
     if(!query4.exec())
         qDebug() << "Query Error: " << query4.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query5 (queryPrep, fqt_test);
     query5.addBindValue("1_12");
     query5.addBindValue("FALSE");
     query5.addBindValue("NULL");
     query5.addBindValue("NULL");
     if(!query5.exec())
         qDebug() << "Query Error: " << query5.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query6 (queryPrep, fqt_test);
     query6.addBindValue("1_14");
     query6.addBindValue("FALSE");
     query6.addBindValue("NULL");
     query6.addBindValue("NULL");
     if(!query6.exec())
         qDebug() << "Query Error: " << query6.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query7 (queryPrep, fqt_test);
     query7.addBindValue("1_15");
     query7.addBindValue("FALSE");
     query7.addBindValue("NULL");
     query7.addBindValue("NULL");
     if(!query7.exec())
         qDebug() << "Query Error: " << query7.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query8 (queryPrep, fqt_test);
     query8.addBindValue("1_16");
     query8.addBindValue("FALSE");
     query8.addBindValue("NULL");
     query8.addBindValue("NULL");
     if(!query8.exec())
         qDebug() << "Query Error: " << query8.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query9 (queryPrep, fqt_test);
     query9.addBindValue("1_2");
     query9.addBindValue("FALSE");
     query9.addBindValue("NULL");
     query9.addBindValue("NULL");
     if(!query9.exec())
         qDebug() << "Query Error: " << query9.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query10 (queryPrep, fqt_test);
     query10.addBindValue("1_3");
     query10.addBindValue("FALSE");
     query10.addBindValue("NULL");
     query10.addBindValue("NULL");
     if(!query10.exec())
         qDebug() << "Query Error: " << query10.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query12 (queryPrep, fqt_test);
     query12.addBindValue("1_4");
     query12.addBindValue("FALSE");
     query12.addBindValue("NULL");
     query12.addBindValue("NULL");
     if(!query12.exec())
         qDebug() << "Query Error: " << query12.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query13 (queryPrep, fqt_test);
     query13.addBindValue("1_5");
     query13.addBindValue("FALSE");
     query13.addBindValue("NULL");
     query13.addBindValue("NULL");
     if(!query13.exec())
         qDebug() << "Query Error: " << query13.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query14 (queryPrep, fqt_test);
     query14.addBindValue("1_6");
     query14.addBindValue("FALSE");
     query14.addBindValue("NULL");
     query14.addBindValue("NULL");
     if(!query14.exec())
         qDebug() << "Query Error: " << query14.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query15 (queryPrep, fqt_test);
     query15.addBindValue("1_7");
     query15.addBindValue("FALSE");
     query15.addBindValue("NULL");
     query15.addBindValue("NULL");
     if(!query15.exec())
         qDebug() << "Query Error: " << query15.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query16 (queryPrep, fqt_test);
     query16.addBindValue("1_8");
     query16.addBindValue("FALSE");
     query16.addBindValue("NULL");
     query16.addBindValue("NULL");
     if(!query16.exec())
         qDebug() << "Query Error: " << query16.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query17 (queryPrep, fqt_test);
     query17.addBindValue("1_9");
     query17.addBindValue("FALSE");
     query17.addBindValue("NULL");
     query17.addBindValue("NULL");
     if(!query17.exec())
         qDebug() << "Query Error: " << query17.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query18 (queryPrep, fqt_test);
     query18.addBindValue("2_1");
     query18.addBindValue("TRUE");
     query18.addBindValue("NULL");
     query18.addBindValue("NULL");
     if(!query18.exec())
         qDebug() << "Query Error: " << query18.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query19 (queryPrep, fqt_test);
     query19.addBindValue("2_10");
     query19.addBindValue("FALSE");
     query19.addBindValue("NULL");
     query19.addBindValue("NULL");
     if(!query19.exec())
         qDebug() << "Query Error: " << query19.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query20 (queryPrep, fqt_test);
     query20.addBindValue("2_11");
     query20.addBindValue("FALSE");
     query20.addBindValue("NULL");
     query20.addBindValue("NULL");
     if(!query20.exec())
         qDebug() << "Query Error: " << query20.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query21 (queryPrep, fqt_test);
     query21.addBindValue("2_12");
     query21.addBindValue("FALSE");
     query21.addBindValue("NULL");
     query21.addBindValue("NULL");
     if(!query21.exec())
         qDebug() << "Query Error: " << query21.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query22 (queryPrep, fqt_test);
     query22.addBindValue("2_13");
     query22.addBindValue("FALSE");
     query22.addBindValue("NULL");
     query22.addBindValue("NULL");
     if(!query22.exec())
         qDebug() << "Query Error: " << query22.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query23 (queryPrep, fqt_test);
     query23.addBindValue("2_14");
     query23.addBindValue("FALSE");
     query23.addBindValue("NULL");
     query23.addBindValue("NULL");
     if(!query23.exec())
         qDebug() << "Query Error: " << query23.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query24 (queryPrep, fqt_test);
     query24.addBindValue("2_15");
     query24.addBindValue("TRUE");
     query24.addBindValue("NULL");
     query24.addBindValue("NULL");
     if(!query24.exec())
         qDebug() << "Query Error: " << query24.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query25 (queryPrep, fqt_test);
     query25.addBindValue("2_16");
     query25.addBindValue("FALSE");
     query25.addBindValue("NULL");
     query25.addBindValue("NULL");
     if(!query25.exec())
         qDebug() << "Query Error: " << query25.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query26 (queryPrep, fqt_test);
     query26.addBindValue("2_2");
     query26.addBindValue("FALSE");
     query26.addBindValue("NULL");
     query26.addBindValue("NULL");
     if(!query26.exec())
         qDebug() << "Query Error: " << query26.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query27 (queryPrep, fqt_test);
     query27.addBindValue("2_3");
     query27.addBindValue("FALSE");
     query27.addBindValue("NULL");
     query27.addBindValue("NULL");
     if(!query27.exec())
         qDebug() << "Query Error: " << query27.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query28 (queryPrep, fqt_test);
     query28.addBindValue("2_4");
     query28.addBindValue("FALSE");
     query28.addBindValue("NULL");
     query28.addBindValue("NULL");
     if(!query28.exec())
         qDebug() << "Query Error: " << query28.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query29 (queryPrep, fqt_test);
     query29.addBindValue("2_5");
     query29.addBindValue("TRUE");
     query29.addBindValue("NULL");
     query29.addBindValue("NULL");
     if(!query29.exec())
         qDebug() << "Query Error: " << query29.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query30 (queryPrep, fqt_test);
     query30.addBindValue("2_6");
     query30.addBindValue("FALSE");
     query30.addBindValue("NULL");
     query30.addBindValue("NULL");
     if(!query30.exec())
         qDebug() << "Query Error: " << query30.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query31 (queryPrep, fqt_test);
     query31.addBindValue("2_7");
     query31.addBindValue("FALSE");
     query31.addBindValue("NULL");
     query31.addBindValue("NULL");
     if(!query31.exec())
         qDebug() << "Query Error: " << query31.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query32 (queryPrep, fqt_test);
     query32.addBindValue("2_8");
     query32.addBindValue("FALSE");
     query32.addBindValue("NULL");
     query32.addBindValue("NULL");
     if(!query32.exec())
         qDebug() << "Query Error: " << query32.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query33 (queryPrep, fqt_test);
     query33.addBindValue("2_9");
     query33.addBindValue("FALSE");
     query33.addBindValue("NULL");
     query33.addBindValue("NULL");
     if(!query33.exec())
         qDebug() << "Query Error: " << query33.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query34 (queryPrep, fqt_test);
     query34.addBindValue("3_1");
     query34.addBindValue("FALSE");
     query34.addBindValue("NULL");
     query34.addBindValue("NULL");
     if(!query34.exec())
         qDebug() << "Query Error: " << query34.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query35 (queryPrep, fqt_test);
     query35.addBindValue("3_13");
     query35.addBindValue("FALSE");
     query35.addBindValue("NULL");
     query35.addBindValue("NULL");
     if(!query35.exec())
         qDebug() << "Query Error: " << query35.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query36 (queryPrep, fqt_test);
     query36.addBindValue("3_14");
     query36.addBindValue("FALSE");
     query36.addBindValue("NULL");
     query36.addBindValue("NULL");
     if(!query36.exec())
         qDebug() << "Query Error: " << query36.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query37 (queryPrep, fqt_test);
     query37.addBindValue("3_15");
     query37.addBindValue("FALSE");
     query37.addBindValue("NULL");
     query37.addBindValue("NULL");
     if(!query37.exec())
         qDebug() << "Query Error: " << query37.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query38 (queryPrep, fqt_test);
     query38.addBindValue("3_16");
     query38.addBindValue("FALSE");
     query38.addBindValue("NULL");
     query38.addBindValue("NULL");
     qDebug() << "Query Error: " << query38.lastError();
     if(!query38.exec())
         qDebug() << "Query Error: " << query38.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query39 (queryPrep, fqt_test);
     query39.addBindValue("3_2");
     query39.addBindValue("FALSE");
     query39.addBindValue("NULL");
     query39.addBindValue("NULL");
     if(!query39.exec())
         qDebug() << "Query Error: " << query39.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query40 (queryPrep, fqt_test);
     query40.addBindValue("3_3");
     query40.addBindValue("FALSE");
     query40.addBindValue("NULL");
     query40.addBindValue("NULL");
     if(!query40.exec())
         qDebug() << "Query Error: " << query40.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query41 (queryPrep, fqt_test);
     query41.addBindValue("3_4");
     query41.addBindValue("FALSE");
     query41.addBindValue("NULL");
     query41.addBindValue("NULL");
     if(!query41.exec())
         qDebug() << "Query Error: " << query41.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query42 (queryPrep, fqt_test);
     query42.addBindValue("3_5");
     query42.addBindValue("FALSE");
     query42.addBindValue("NULL");
     query42.addBindValue("NULL");
     if(!query42.exec())
         qDebug() << "Query Error: " << query42.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query43 (queryPrep, fqt_test);
     query43.addBindValue("3_6");
     query43.addBindValue("FALSE");
     query43.addBindValue("NULL");
     query43.addBindValue("NULL");
     if(!query43.exec())
         qDebug() << "Query Error: " << query43.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query44 (queryPrep, fqt_test);
     query44.addBindValue("3_7");
     query44.addBindValue("FALSE");
     query44.addBindValue("NULL");
     query44.addBindValue("NULL");
     if(!query44.exec())
         qDebug() << "Query Error: " << query44.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query45 (queryPrep, fqt_test);
     query45.addBindValue("3_8");
     query45.addBindValue("FALSE");
     query45.addBindValue("NULL");
     query45.addBindValue("NULL");
     if(!query45.exec())
         qDebug() << "Query Error: " << query45.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query46 (queryPrep, fqt_test);
     query46.addBindValue("4_10");
     query46.addBindValue("FALSE");
     query46.addBindValue("NULL");
     query46.addBindValue("NULL");
     if(!query46.exec())
         qDebug() << "Query Error: " << query46.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query47 (queryPrep, fqt_test);
     query47.addBindValue("4_11");
     query47.addBindValue("FALSE");
     query47.addBindValue("NULL");
     query47.addBindValue("NULL");
     if(!query47.exec())
         qDebug() << "Query Error: " << query47.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query48 (queryPrep, fqt_test);
     query48.addBindValue("4_12");
     query48.addBindValue("FALSE");
     query48.addBindValue("NULL");
     query48.addBindValue("NULL");
     if(!query48.exec())
         qDebug() << "Query Error: " << query48.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query49 (queryPrep, fqt_test);
     query49.addBindValue("4_13");
     query49.addBindValue("FALSE");
     query49.addBindValue("NULL");
     query49.addBindValue("NULL");
     if(!query49.exec())
         qDebug() << "Query Error: " << query49.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query50 (queryPrep, fqt_test);
     query50.addBindValue("4_14");
     query50.addBindValue("FALSE");
     query50.addBindValue("NULL");
     query50.addBindValue("NULL");
     if(!query50.exec())
         qDebug() << "Query Error: " << query50.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query51 (queryPrep, fqt_test);
     query51.addBindValue("4_15");
     query51.addBindValue("FALSE");
     query51.addBindValue("NULL");
     query51.addBindValue("NULL");
     if(!query51.exec())
         qDebug() << "Query Error: " << query51.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query52 (queryPrep, fqt_test);
     query52.addBindValue("4_16");
     query52.addBindValue("FALSE");
     query52.addBindValue("NULL");
     query52.addBindValue("NULL");
     if(!query52.exec())
         qDebug() << "Query Error: " << query52.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query53 (queryPrep, fqt_test);
     query53.addBindValue("4_2");
     query53.addBindValue("FALSE");
     query53.addBindValue("NULL");
     query53.addBindValue("NULL");
     if(!query53.exec())
         qDebug() << "Query Error: " << query53.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query54 (queryPrep, fqt_test);
     query54.addBindValue("4_3");
     query54.addBindValue("FALSE");
     query54.addBindValue("NULL");
     query54.addBindValue("NULL");
     if(!query54.exec())
         qDebug() << "Query Error: " << query54.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query55 (queryPrep, fqt_test);
     query55.addBindValue("4_4");
     query55.addBindValue("FALSE");
     query55.addBindValue("NULL");
     query55.addBindValue("NULL");
     if(!query55.exec())
         qDebug() << "Query Error: " << query55.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query56 (queryPrep, fqt_test);
     query56.addBindValue("4_9");
     query56.addBindValue("FALSE");
     query56.addBindValue("NULL");
     query56.addBindValue("NULL");
     if(!query56.exec())
         qDebug() << "Query Error: " << query56.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query57 (queryPrep, fqt_test);
     query57.addBindValue("5_1");
     query57.addBindValue("FALSE");
     query57.addBindValue("NULL");
     query57.addBindValue("NULL");
     if(!query57.exec())
         qDebug() << "Query Error: " << query57.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query58 (queryPrep, fqt_test);
     query58.addBindValue("5_13");
     query58.addBindValue("FALSE");
     query58.addBindValue("NULL");
     query58.addBindValue("NULL");
     if(!query58.exec())
         qDebug() << "Query Error: " << query58.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query59 (queryPrep, fqt_test);
     query59.addBindValue("5_14");
     query59.addBindValue("FALSE");
     query59.addBindValue("NULL");
     query59.addBindValue("NULL");
     if(!query59.exec())
         qDebug() << "Query Error: " << query59.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query60 (queryPrep, fqt_test);
     query60.addBindValue("5_15");
     query60.addBindValue("FALSE");
     query60.addBindValue("NULL");
     query60.addBindValue("NULL");
     if(!query60.exec())
         qDebug() << "Query Error: " << query60.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query61 (queryPrep, fqt_test);
     query61.addBindValue("5_16");
     query61.addBindValue("FALSE");
     query61.addBindValue("NULL");
     query61.addBindValue("NULL");
     if(!query61.exec())
         qDebug() << "Query Error: " << query61.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query62 (queryPrep, fqt_test);
     query62.addBindValue("5_2");
     query62.addBindValue("FALSE");
     query62.addBindValue("NULL");
     query62.addBindValue("NULL");
     if(!query62.exec())
         qDebug() << "Query Error: " << query62.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query63 (queryPrep, fqt_test);
     query63.addBindValue("5_3");
     query63.addBindValue("FALSE");
     query63.addBindValue("NULL");
     query63.addBindValue("NULL");
     if(!query63.exec())
         qDebug() << "Query Error: " << query63.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query64 (queryPrep, fqt_test);
     query64.addBindValue("5_4");
     query64.addBindValue("FALSE");
     query64.addBindValue("NULL");
     query64.addBindValue("NULL");
     if(!query64.exec())
         qDebug() << "Query Error: " << query64.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query65 (queryPrep, fqt_test);
     query65.addBindValue("5_5");
     query65.addBindValue("FALSE");
     query65.addBindValue("NULL");
     query65.addBindValue("NULL");
     if(!query65.exec())
         qDebug() << "Query Error: " << query65.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query66 (queryPrep, fqt_test);
     query66.addBindValue("5_6");
     query66.addBindValue("FALSE");
     query66.addBindValue("NULL");
     query66.addBindValue("NULL");
     if(!query66.exec())
         qDebug() << "Query Error: " << query66.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query67 (queryPrep, fqt_test);
     query67.addBindValue("5_7");
     query67.addBindValue("FALSE");
     query67.addBindValue("NULL");
     query67.addBindValue("NULL");
     if(!query67.exec())
         qDebug() << "Query Error: " << query67.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query68 (queryPrep, fqt_test);
     query68.addBindValue("5_8");
     query68.addBindValue("FALSE");
     query68.addBindValue("NULL");
     query68.addBindValue("NULL");
     if(!query68.exec())
         qDebug() << "Query Error: " << query68.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query69 (queryPrep, fqt_test);
     query69.addBindValue("6_1");
     query69.addBindValue("FALSE");
     query69.addBindValue("NULL");
     query69.addBindValue("NULL");
     if(!query69.exec())
         qDebug() << "Query Error: " << query69.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query70 (queryPrep, fqt_test);
     query70.addBindValue("6_2");
     query70.addBindValue("FALSE");
     query70.addBindValue("NULL");
     query70.addBindValue("NULL");
     if(!query70.exec())
         qDebug() << "Query Error: " << query70.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query71 (queryPrep, fqt_test);
     query71.addBindValue("6_3");
     query71.addBindValue("FALSE");
     query71.addBindValue("NULL");
     query71.addBindValue("NULL");
     if(!query71.exec())
         qDebug() << "Query Error: " << query71.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query72 (queryPrep, fqt_test);
     query72.addBindValue("6_4");
     query72.addBindValue("FALSE");
     query72.addBindValue("NULL");
     query72.addBindValue("NULL");
     if(!query72.exec())
         qDebug() << "Query Error: " << query72.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query73 (queryPrep, fqt_test);
     query73.addBindValue("6_5");
     query73.addBindValue("FALSE");
     query73.addBindValue("NULL");
     query73.addBindValue("NULL");
     if(!query73.exec())
         qDebug() << "Query Error: " << query73.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query74 (queryPrep, fqt_test);
     query74.addBindValue("6_6");
     query74.addBindValue("FALSE");
     query74.addBindValue("NULL");
     query74.addBindValue("NULL");
     if(!query74.exec())
         qDebug() << "Query Error: " << query74.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query75 (queryPrep, fqt_test);
     query75.addBindValue("6_7");
     query75.addBindValue("FALSE");
     query75.addBindValue("NULL");
     query75.addBindValue("NULL");
     if(!query75.exec())
         qDebug() << "Query Error: " << query75.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query76 (queryPrep, fqt_test);
     query76.addBindValue("6_8");
     query76.addBindValue("FALSE");
     query76.addBindValue("NULL");
     query76.addBindValue("NULL");
     if(!query76.exec())
         qDebug() << "Query Error: " << query76.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query77 (queryPrep, fqt_test);
     query77.addBindValue("7_1");
     query77.addBindValue("FALSE");
     query77.addBindValue("NULL");
     query77.addBindValue("NULL");
     if(!query77.exec())
         qDebug() << "Query Error: " << query77.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query78 (queryPrep, fqt_test);
     query78.addBindValue("7_13");
     query78.addBindValue("FALSE");
     query78.addBindValue("NULL");
     query78.addBindValue("NULL");
     if(!query78.exec())
         qDebug() << "Query Error: " << query78.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query79 (queryPrep, fqt_test);
     query79.addBindValue("7_2");
     query79.addBindValue("FALSE");
     query79.addBindValue("NULL");
     query79.addBindValue("NULL");
     if(!query79.exec())
         qDebug() << "Query Error: " << query79.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query80 (queryPrep, fqt_test);
     query80.addBindValue("7_3");
     query80.addBindValue("FALSE");
     query80.addBindValue("NULL");
     query80.addBindValue("NULL");
     if(!query80.exec())
         qDebug() << "Query Error: " << query80.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query81 (queryPrep, fqt_test);
     query81.addBindValue("7_4");
     query81.addBindValue("FALSE");
     query81.addBindValue("NULL");
     query81.addBindValue("NULL");
     if(!query81.exec())
         qDebug() << "Query Error: " << query81.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query82 (queryPrep, fqt_test);
     query82.addBindValue("7_5");
     query82.addBindValue("FALSE");
     query82.addBindValue("NULL");
     query82.addBindValue("NULL");
     if(!query82.exec())
         qDebug() << "Query Error: " << query82.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query83 (queryPrep, fqt_test);
     query83.addBindValue("7_6");
     query83.addBindValue("FALSE");
     query83.addBindValue("NULL");
     query83.addBindValue("NULL");
     if(!query83.exec())
         qDebug() << "Query Error: " << query83.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query84 (queryPrep, fqt_test);
     query84.addBindValue("7_7");
     query84.addBindValue("FALSE");
     query84.addBindValue("NULL");
     query84.addBindValue("NULL");
     if(!query84.exec())
         qDebug() << "Query Error: " << query84.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query85 (queryPrep, fqt_test);
     query85.addBindValue("7_8");
     query85.addBindValue("FALSE");
     query85.addBindValue("NULL");
     query85.addBindValue("NULL");
     if(!query85.exec())
         qDebug() << "Query Error: " << query85.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query86 (queryPrep, fqt_test);
     query86.addBindValue("8_1");
     query86.addBindValue("FALSE");
     query86.addBindValue("NULL");
     query86.addBindValue("NULL");
     if(!query86.exec())
         qDebug() << "Query Error: " << query86.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query87 (queryPrep, fqt_test);
     query87.addBindValue("8_10");
     query87.addBindValue("FALSE");
     query87.addBindValue("NULL");
     query87.addBindValue("NULL");
     if(!query87.exec())
         qDebug() << "Query Error: " << query87.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query88 (queryPrep, fqt_test);
     query88.addBindValue("8_11");
     query88.addBindValue("FALSE");
     query88.addBindValue("NULL");
     query88.addBindValue("NULL");
     if(!query88.exec())
         qDebug() << "Query Error: " << query88.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query89 (queryPrep, fqt_test);
     query89.addBindValue("8_12");
     query89.addBindValue("FALSE");
     query89.addBindValue("NULL");
     query89.addBindValue("NULL");
     if(!query89.exec())
         qDebug() << "Query Error: " << query89.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query90 (queryPrep, fqt_test);
     query90.addBindValue("8_13");
     query90.addBindValue("FALSE");
     query90.addBindValue("NULL");
     query90.addBindValue("NULL");
     if(!query90.exec())
         qDebug() << "Query Error: " << query90.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query91 (queryPrep, fqt_test);
     query91.addBindValue("8_14");
     query91.addBindValue("FALSE");
     query91.addBindValue("NULL");
     query91.addBindValue("NULL");
     if(!query91.exec())
         qDebug() << "Query Error: " << query91.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query92 (queryPrep, fqt_test);
     query92.addBindValue("8_15");
     query92.addBindValue("FALSE");
     query92.addBindValue("NULL");
     query92.addBindValue("NULL");
     if(!query92.exec())
         qDebug() << "Query Error: " << query92.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query93 (queryPrep, fqt_test);
     query93.addBindValue("8_16");
     query93.addBindValue("FALSE");
     query93.addBindValue("NULL");
     query93.addBindValue("NULL");
     if(!query93.exec())
         qDebug() << "Query Error: " << query93.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query94 (queryPrep, fqt_test);
     query94.addBindValue("8_2");
     query94.addBindValue("FALSE");
     query94.addBindValue("NULL");
     query94.addBindValue("NULL");
     if(!query94.exec())
         qDebug() << "Query Error: " << query94.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query95 (queryPrep, fqt_test);
     query95.addBindValue("8_3");
     query95.addBindValue("FALSE");
     query95.addBindValue("NULL");
     query95.addBindValue("NULL");
     if(!query95.exec())
         qDebug() << "Query Error: " << query95.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query96 (queryPrep, fqt_test);
     query96.addBindValue("8_4");
     query96.addBindValue("FALSE");
     query96.addBindValue("NULL");
     query96.addBindValue("NULL");
     if(!query96.exec())
         qDebug() << "Query Error: " << query96.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query97 (queryPrep, fqt_test);
     query97.addBindValue("8_5");
     query97.addBindValue("FALSE");
     query97.addBindValue("NULL");
     query97.addBindValue("NULL");
     if(!query97.exec())
         qDebug() << "Query Error: " << query97.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query98 (queryPrep, fqt_test);
     query98.addBindValue("8_6");
     query98.addBindValue("FALSE");
     query98.addBindValue("NULL");
     query98.addBindValue("NULL");
     if(!query98.exec())
         qDebug() << "Query Error: " << query98.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query99 (queryPrep, fqt_test);
     query99.addBindValue("8_7");
     query99.addBindValue("FALSE");
     query99.addBindValue("NULL");
     query99.addBindValue("NULL");
     if(!query99.exec())
         qDebug() << "Query Error: " << query99.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query100 (queryPrep, fqt_test);
     query100.addBindValue("8_8");
     query100.addBindValue("FALSE");
     query100.addBindValue("NULL");
     query100.addBindValue("NULL");
     if(!query100.exec())
         qDebug() << "Query Error: " << query100.lastError();

     queryPrep = "INSERT INTO track_ds VALUES(?,?,?,?)";
     QSqlQuery query101 (queryPrep, fqt_test);
     query101.addBindValue("8_9");
     query101.addBindValue("FALSE");
     query101.addBindValue("NULL");
     query101.addBindValue("NULL");
     if(!query101.exec())
         qDebug() << "Query Error: " << query101.lastError();


     queryPrep = "CREATE TABLE track_power (ds TEXT, state BOOLEAN)";
     QSqlQuery query110 (queryPrep, fqt_test);
     if(!query110.exec())
         qDebug() << "Query Error: " << query110.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?, ?)";
     QSqlQuery query111 (queryPrep, fqt_test);
     query111.addBindValue("1_1");
     query111.addBindValue(1);
     if(!query111.exec())
         qDebug() << "Query Error: " << query111.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query112 (queryPrep, fqt_test);
     query112.addBindValue("1_10");
     query112.addBindValue(1);
     if(!query112.exec())
         qDebug() << "Query Error: " << query112.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query113 (queryPrep, fqt_test);
     query113.addBindValue("1_11");
     query113.addBindValue(1);
     if(!query113.exec())
         qDebug() << "Query Error: " << query113.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query114 (queryPrep, fqt_test);
     query114.addBindValue("1_12");
     query114.addBindValue(1);
     if(!query114.exec())
         qDebug() << "Query Error: " << query114.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query115 (queryPrep, fqt_test);
     query115.addBindValue("1_14");
     query115.addBindValue(1);
     if(!query115.exec())
         qDebug() << "Query Error: " << query115.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query116 (queryPrep, fqt_test);
     query116.addBindValue("1_15");
     query116.addBindValue(1);
     if(!query116.exec())
         qDebug() << "Query Error: " << query116.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query117 (queryPrep, fqt_test);
     query117.addBindValue("1_16");
     query117.addBindValue(1);
     if(!query117.exec())
         qDebug() << "Query Error: " << query117.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query118 (queryPrep, fqt_test);
     query118.addBindValue("1_2");
     query118.addBindValue(1);
     if(!query118.exec())
         qDebug() << "Query Error: " << query118.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query119 (queryPrep, fqt_test);
     query119.addBindValue("1_3");
     query119.addBindValue(1);
     if(!query119.exec())
         qDebug() << "Query Error: " << query119.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query120 (queryPrep, fqt_test);
     query120.addBindValue("1_4");
     query120.addBindValue(1);
     if(!query120.exec())
         qDebug() << "Query Error: " << query120.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query121 (queryPrep, fqt_test);
     query121.addBindValue("1_5");
     query121.addBindValue(1);
     if(!query121.exec())
         qDebug() << "Query Error: " << query121.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query122 (queryPrep, fqt_test);
     query122.addBindValue("1_6");
     query122.addBindValue(1);
     if(!query122.exec())
         qDebug() << "Query Error: " << query122.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query123 (queryPrep, fqt_test);
     query123.addBindValue("1_7");
     query123.addBindValue(1);
     if(!query123.exec())
         qDebug() << "Query Error: " << query123.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query124 (queryPrep, fqt_test);
     query124.addBindValue("1_8");
     query124.addBindValue(1);
     if(!query124.exec())
         qDebug() << "Query Error: " << query124.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query125 (queryPrep, fqt_test);
     query125.addBindValue("1_9");
     query125.addBindValue(1);
     if(!query125.exec())
         qDebug() << "Query Error: " << query125.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query126 (queryPrep, fqt_test);
     query126.addBindValue("2_1");
     query126.addBindValue(1);
     if(!query126.exec())
         qDebug() << "Query Error: " << query126.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query127 (queryPrep, fqt_test);
     query127.addBindValue("2_10");
     query127.addBindValue(1);
     if(!query127.exec())
         qDebug() << "Query Error: " << query127.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query128 (queryPrep, fqt_test);
     query128.addBindValue("2_11");
     query128.addBindValue(1);
     if(!query128.exec())
         qDebug() << "Query Error: " << query128.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query129 (queryPrep, fqt_test);
     query129.addBindValue("2_12");
     query129.addBindValue(1);
     if(!query129.exec())
         qDebug() << "Query Error: " << query129.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query130 (queryPrep, fqt_test);
     query130.addBindValue("2_13");
     query130.addBindValue(1);
     if(!query130.exec())
         qDebug() << "Query Error: " << query130.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query131 (queryPrep, fqt_test);
     query131.addBindValue("2_14");
     query131.addBindValue(1);
     if(!query131.exec())
         qDebug() << "Query Error: " << query131.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query132 (queryPrep, fqt_test);
     query132.addBindValue("2_15");
     query132.addBindValue(1);
     if(!query132.exec())
         qDebug() << "Query Error: " << query132.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query133 (queryPrep, fqt_test);
     query133.addBindValue("2_16");
     query133.addBindValue(1);
     if(!query133.exec())
         qDebug() << "Query Error: " << query133.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query134 (queryPrep, fqt_test);
     query134.addBindValue("2_2");
     query134.addBindValue(1);
     if(!query134.exec())
         qDebug() << "Query Error: " << query134.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query135 (queryPrep, fqt_test);
     query135.addBindValue("2_3");
     query135.addBindValue(1);
     if(!query135.exec())
         qDebug() << "Query Error: " << query135.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query136 (queryPrep, fqt_test);
     query136.addBindValue("2_4");
     query136.addBindValue(1);
     if(!query136.exec())
         qDebug() << "Query Error: " << query136.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query137 (queryPrep, fqt_test);
     query137.addBindValue("2_5");
     query137.addBindValue(1);
     if(!query137.exec())
         qDebug() << "Query Error: " << query137.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query138 (queryPrep, fqt_test);
     query138.addBindValue("2_6");
     query138.addBindValue(1);
     if(!query138.exec())
         qDebug() << "Query Error: " << query138.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query139 (queryPrep, fqt_test);
     query139.addBindValue("2_7");
     query139.addBindValue(1);
     if(!query139.exec())
         qDebug() << "Query Error: " << query139.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query140 (queryPrep, fqt_test);
     query140.addBindValue("2_8");
     query140.addBindValue(1);
     if(!query140.exec())
         qDebug() << "Query Error: " << query140.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query141 (queryPrep, fqt_test);
     query141.addBindValue("2_9");
     query141.addBindValue(1);
     if(!query141.exec())
         qDebug() << "Query Error: " << query141.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query142 (queryPrep, fqt_test);
     query142.addBindValue("3_1");
     query142.addBindValue(1);
     if(!query142.exec())
         qDebug() << "Query Error: " << query142.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query143 (queryPrep, fqt_test);
     query143.addBindValue("3_13");
     query143.addBindValue(0);
     if(!query143.exec())
         qDebug() << "Query Error: " << query143.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query144 (queryPrep, fqt_test);
     query144.addBindValue("3_14");
     query144.addBindValue(1);
     if(!query144.exec())
         qDebug() << "Query Error: " << query144.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query145 (queryPrep, fqt_test);
     query145.addBindValue("3_15");
     query145.addBindValue(1);
     if(!query145.exec())
         qDebug() << "Query Error: " << query145.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query146 (queryPrep, fqt_test);
     query146.addBindValue("3_16");
     query146.addBindValue(1);
     if(!query146.exec())
         qDebug() << "Query Error: " << query146.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query147 (queryPrep, fqt_test);
     query147.addBindValue("3_2");
     query147.addBindValue(1);
     if(!query147.exec())
         qDebug() << "Query Error: " << query147.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query148 (queryPrep, fqt_test);
     query148.addBindValue("3_3");
     query148.addBindValue(1);
     if(!query148.exec())
         qDebug() << "Query Error: " << query148.lastError();

     queryPrep = "INSERT INTO track_power VALUES ?,?)";
     QSqlQuery query149 (queryPrep, fqt_test);
     query149.addBindValue("3_4");
     query149.addBindValue(1);
     if(!query149.exec())
         qDebug() << "Query Error: " << query149.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query150 (queryPrep, fqt_test);
     query150.addBindValue("3_5");
     query150.addBindValue(1);
     if(!query150.exec())
         qDebug() << "Query Error: " << query150.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query151 (queryPrep, fqt_test);
     query151.addBindValue("3_6");
     query151.addBindValue(1);
     if(!query151.exec())
         qDebug() << "Query Error: " << query151.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query152 (queryPrep, fqt_test);
     query152.addBindValue("3_7");
     query152.addBindValue(1);
     if(!query152.exec())
         qDebug() << "Query Error: " << query152.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query153 (queryPrep, fqt_test);
     query153.addBindValue("3_8");
     query153.addBindValue(0);
     if(!query153.exec())
         qDebug() << "Query Error: " << query153.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query154 (queryPrep, fqt_test);
     query154.addBindValue("4_10");
     query154.addBindValue(1);
     if(!query154.exec())
         qDebug() << "Query Error: " << query154.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query155 (queryPrep, fqt_test);
     query155.addBindValue("4_11");
     query155.addBindValue(1);
     if(!query155.exec())
         qDebug() << "Query Error: " << query155.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query156 (queryPrep, fqt_test);
     query156.addBindValue("4_12");
     query156.addBindValue(1);
     if(!query156.exec())
         qDebug() << "Query Error: " << query156.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query157 (queryPrep, fqt_test);
     query157.addBindValue("4_13");
     query157.addBindValue(1);
     if(!query157.exec())
         qDebug() << "Query Error: " << query157.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query158 (queryPrep, fqt_test);
     query158.addBindValue("4_14");
     query158.addBindValue(1);
     if(!query158.exec())
         qDebug() << "Query Error: " << query158.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query159 (queryPrep, fqt_test);
     query159.addBindValue("4_15");
     query159.addBindValue(1);
     if(!query159.exec())
         qDebug() << "Query Error: " << query159.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query160 (queryPrep, fqt_test);
     query160.addBindValue("4_16");
     query160.addBindValue(1);
     if(!query160.exec())
         qDebug() << "Query Error: " << query160.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query161 (queryPrep, fqt_test);
     query161.addBindValue("4_2");
     query161.addBindValue(1);
     if(!query161.exec())
         qDebug() << "Query Error: " << query161.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query162 (queryPrep, fqt_test);
     query162.addBindValue("4_3");
     query162.addBindValue(1);
     if(!query162.exec())
         qDebug() << "Query Error: " << query162.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query163 (queryPrep, fqt_test);
     query163.addBindValue("4_4");
     query163.addBindValue(1);
     if(!query163.exec())
         qDebug() << "Query Error: " << query163.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query164 (queryPrep, fqt_test);
     query164.addBindValue("4_9");
     query164.addBindValue(1);
     if(!query164.exec())
         qDebug() << "Query Error: " << query164.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query165 (queryPrep, fqt_test);
     query165.addBindValue("5_1");
     query165.addBindValue(1);
     if(!query165.exec())
         qDebug() << "Query Error: " << query165.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query166 (queryPrep, fqt_test);
     query166.addBindValue("5_13");
     query166.addBindValue(1);
     if(!query166.exec())
         qDebug() << "Query Error: " << query166.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query167 (queryPrep, fqt_test);
     query167.addBindValue("5_14");
     query167.addBindValue(1);
     if(!query167.exec())
         qDebug() << "Query Error: " << query167.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query168 (queryPrep, fqt_test);
     query168.addBindValue("5_15");
     query168.addBindValue(1);
     if(!query168.exec())
         qDebug() << "Query Error: " << query168.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query169 (queryPrep, fqt_test);
     query169.addBindValue("5_16");
     query169.addBindValue(1);
     if(!query169.exec())
         qDebug() << "Query Error: " << query169.lastError();

     queryPrep = "INSERT INTO track_power VALUES ?,?)";
     QSqlQuery query170 (queryPrep, fqt_test);
     query170.addBindValue("5_2");
     query170.addBindValue(1);
     if(!query170.exec())
         qDebug() << "Query Error: " << query170.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query171 (queryPrep, fqt_test);
     query171.addBindValue("5_3");
     query171.addBindValue(1);
     if(!query171.exec())
         qDebug() << "Query Error: " << query171.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query172 (queryPrep, fqt_test);
     query172.addBindValue("5_4");
     query172.addBindValue(0);
     if(!query172.exec())
         qDebug() << "Query Error: " << query172.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query173 (queryPrep, fqt_test);
     query173.addBindValue("5_5");
     query173.addBindValue(1);
     if(!query173.exec())
         qDebug() << "Query Error: " << query173.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query174 (queryPrep, fqt_test);
     query174.addBindValue("5_6");
     query174.addBindValue(1);
     if(!query174.exec())
         qDebug() << "Query Error: " << query174.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query175 (queryPrep, fqt_test);
     query175.addBindValue("5_7");
     query175.addBindValue(1);
     if(!query175.exec())
         qDebug() << "Query Error: " << query175.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query176 (queryPrep, fqt_test);
     query176.addBindValue("5_8");
     query176.addBindValue(1);
     if(!query176.exec())
         qDebug() << "Query Error: " << query176.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query177 (queryPrep, fqt_test);
     query177.addBindValue("6_1");
     query177.addBindValue(1);
     if(!query177.exec())
         qDebug() << "Query Error: " << query177.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query178 (queryPrep, fqt_test);
     query178.addBindValue("6_2");
     query178.addBindValue(1);
     if(!query178.exec())
         qDebug() << "Query Error: " << query178.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query179 (queryPrep, fqt_test);
     query179.addBindValue("6_3");
     query179.addBindValue(1);
     if(!query179.exec())
         qDebug() << "Query Error: " << query179.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query180 (queryPrep, fqt_test);
     query180.addBindValue("6_4");
     query180.addBindValue(1);
     if(!query180.exec())
         qDebug() << "Query Error: " << query180.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query181 (queryPrep, fqt_test);
     query181.addBindValue("6_5");
     query181.addBindValue(1);
     if(!query181.exec())
         qDebug() << "Query Error: " << query181.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query182 (queryPrep, fqt_test);
     query182.addBindValue("6_6");
     query182.addBindValue(1);
     if(!query182.exec())
         qDebug() << "Query Error: " << query182.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query183 (queryPrep, fqt_test);
     query183.addBindValue("6_7");
     query183.addBindValue(1);
     if(!query183.exec())
         qDebug() << "Query Error: " << query183.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query184 (queryPrep, fqt_test);
     query184.addBindValue("6_8");
     query184.addBindValue(1);
     if(!query184.exec())
         qDebug() << "Query Error: " << query184.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query185 (queryPrep, fqt_test);
     query185.addBindValue("7_1");
     query185.addBindValue(1);
     if(!query185.exec())
         qDebug() << "Query Error: " << query185.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query186 (queryPrep, fqt_test);
     query186.addBindValue("7_13");
     query186.addBindValue(1);
     if(!query186.exec())
         qDebug() << "Query Error: " << query186.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query187 (queryPrep, fqt_test);
     query187.addBindValue("7_2");
     query187.addBindValue(1);
     if(!query187.exec())
         qDebug() << "Query Error: " << query187.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query188 (queryPrep, fqt_test);
     query188.addBindValue("7_3");
     query188.addBindValue(1);
     if(!query188.exec())
         qDebug() << "Query Error: " << query188.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query189 (queryPrep, fqt_test);
     query189.addBindValue("7_4");
     query189.addBindValue(1);
     if(!query189.exec())
         qDebug() << "Query Error: " << query189.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query190 (queryPrep, fqt_test);
     query190.addBindValue("7_5");
     query190.addBindValue(1);
     if(!query190.exec())
         qDebug() << "Query Error: " << query190.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query191 (queryPrep, fqt_test);
     query191.addBindValue("7_6");
     query191.addBindValue(1);
     if(!query191.exec())
         qDebug() << "Query Error: " << query191.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query192 (queryPrep, fqt_test);
     query192.addBindValue("7_7");
     query192.addBindValue(1);
     if(!query192.exec())
         qDebug() << "Query Error: " << query192.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query193 (queryPrep, fqt_test);
     query193.addBindValue("7_8");
     query193.addBindValue(1);
     if(!query193.exec())
         qDebug() << "Query Error: " << query193.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query194 (queryPrep, fqt_test);
     query194.addBindValue("8_1");
     query194.addBindValue(0);
     if(!query194.exec())
         qDebug() << "Query Error: " << query194.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query195 (queryPrep, fqt_test);
     query195.addBindValue("8_10");
     query195.addBindValue(1);
     if(!query195.exec())
         qDebug() << "Query Error: " << query195.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query196 (queryPrep, fqt_test);
     query196.addBindValue("8_11");
     query196.addBindValue(1);
     if(!query196.exec())
         qDebug() << "Query Error: " << query196.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query197 (queryPrep, fqt_test);
     query197.addBindValue("8_12");
     query197.addBindValue(1);
     if(!query197.exec())
         qDebug() << "Query Error: " << query197.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?))";
     QSqlQuery query198 (queryPrep, fqt_test);
     query198.addBindValue("8_13");
     query198.addBindValue(1);
     if(!query198.exec())
         qDebug() << "Query Error: " << query198.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query199 (queryPrep, fqt_test);
     query199.addBindValue("8_14");
     query199.addBindValue(1);
     if(!query199.exec())
         qDebug() << "Query Error: " << query199.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query200 (queryPrep, fqt_test);
     query200.addBindValue("8_15");
     query200.addBindValue(1);
     if(!query200.exec())
         qDebug() << "Query Error: " << query200.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query201 (queryPrep, fqt_test);
     query201.addBindValue("8_16");
     query201.addBindValue(1);
     if(!query201.exec())
         qDebug() << "Query Error: " << query201.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query202 (queryPrep, fqt_test);
     query202.addBindValue("8_2");
     query202.addBindValue(1);
     if(!query202.exec())
         qDebug() << "Query Error: " << query202.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query203 (queryPrep, fqt_test);
     query203.addBindValue("8_3");
     query203.addBindValue(1);
     if(!query203.exec())
         qDebug() << "Query Error: " << query203.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query204 (queryPrep, fqt_test);
     query204.addBindValue("8_4");
     query204.addBindValue(1);
     if(!query204.exec())
         qDebug() << "Query Error: " << query204.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query205 (queryPrep, fqt_test);
     query205.addBindValue("8_5");
     query205.addBindValue(1);
     if(!query205.exec())
         qDebug() << "Query Error: " << query205.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query206 (queryPrep, fqt_test);
     query206.addBindValue("8_6");
     query206.addBindValue(1);
     if(!query206.exec())
         qDebug() << "Query Error: " << query206.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query207 (queryPrep, fqt_test);
     query207.addBindValue("8_7");
     query207.addBindValue(1);
     if(!query207.exec())
         qDebug() << "Query Error: " << query207.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query208 (queryPrep, fqt_test);
     query208.addBindValue("8_8");
     query208.addBindValue(1);
     if(!query208.exec())
         qDebug() << "Query Error: " << query208.lastError();

     queryPrep = "INSERT INTO track_power VALUES (?,?)";
     QSqlQuery query209 (queryPrep, fqt_test);
     query209.addBindValue("8_9");
     query209.addBindValue(1);
     if(!query209.exec())
         qDebug() << "Query Error: " << query209.lastError();


     queryPrep = "CREATE TABLE track_switch_5a (id INTEGER, vertex INTEGER, position TINYINT)";
     QSqlQuery query210 (queryPrep, fqt_test);
     if(!query210.exec())
         qDebug() << "Query Error: " << query210.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query211 (queryPrep, fqt_test);
         query211.addBindValue(11);
         query211.addBindValue(14);
         query211.addBindValue(0);
     if(!query211.exec())
         qDebug() << "Query Error: " << query211.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query212 (queryPrep, fqt_test);
         query212.addBindValue(12);
         query212.addBindValue(45);
         query212.addBindValue(1);
     if(!query212.exec())
         qDebug() << "Query Error: " << query212.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query213 (queryPrep, fqt_test);
         query213.addBindValue(13);
         query213.addBindValue(25);
         query213.addBindValue(1);
     if(!query213.exec())
         qDebug() << "Query Error: " << query213.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query214 (queryPrep, fqt_test);
         query214.addBindValue(14);
         query214.addBindValue(14);
         query214.addBindValue(1);
     if(!query214.exec())
         qDebug() << "Query Error: " << query214.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query215 (queryPrep, fqt_test);
         query215.addBindValue(42);
         query215.addBindValue(42);
         query215.addBindValue(1);
     if(!query215.exec())
         qDebug() << "Query Error: " << query215.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query216 (queryPrep, fqt_test);
         query216.addBindValue(43);
         query216.addBindValue(43);
         query216.addBindValue(1);
     if(!query216.exec())
         qDebug() << "Query Error: " << query216.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query217 (queryPrep, fqt_test);
         query217.addBindValue(21);
         query217.addBindValue(21);
         query217.addBindValue(1);
     if(!query217.exec())
         qDebug() << "Query Error: " << query217.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query218 (queryPrep, fqt_test);
         query218.addBindValue(22);
         query218.addBindValue(22);
         query218.addBindValue(1);
     if(!query218.exec())
         qDebug() << "Query Error: " << query218.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query219 (queryPrep, fqt_test);
         query219.addBindValue(23);
         query219.addBindValue(23);
         query219.addBindValue(1);
     if(!query219.exec())
         qDebug() << "Query Error: " << query219.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query220 (queryPrep, fqt_test);
         query220.addBindValue(24);
         query220.addBindValue(24);
         query220.addBindValue(1);
     if(!query220.exec())
         qDebug() << "Query Error: " << query220.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query221 (queryPrep, fqt_test);
         query221.addBindValue(31);
         query221.addBindValue(31);
         query221.addBindValue(1);
     if(!query221.exec())
         qDebug() << "Query Error: " << query221.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query222 (queryPrep, fqt_test);
         query222.addBindValue(32);
         query222.addBindValue(32);
         query222.addBindValue(1);
     if(!query222.exec())
         qDebug() << "Query Error: " << query222.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query223 (queryPrep, fqt_test);
         query223.addBindValue(33);
         query223.addBindValue(33);
         query223.addBindValue(1);
     if(!query223.exec())
         qDebug() << "Query Error: " << query223.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query224 (queryPrep, fqt_test);
         query224.addBindValue(34);
         query224.addBindValue(34);
         query224.addBindValue(1);
     if(!query224.exec())
         qDebug() << "Query Error: " << query224.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query225 (queryPrep, fqt_test);
         query225.addBindValue(41);
         query225.addBindValue(41);
         query225.addBindValue(1);
     if(!query225.exec())
         qDebug() << "Query Error: " << query225.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query226 (queryPrep, fqt_test);
         query226.addBindValue(61);
         query226.addBindValue(61);
         query226.addBindValue(1);
     if(!query226.exec())
         qDebug() << "Query Error: " << query226.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query227 (queryPrep, fqt_test);
         query227.addBindValue(62);
         query227.addBindValue(62);
         query227.addBindValue(1);
     if(!query227.exec())
         qDebug() << "Query Error: " << query227.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query228 (queryPrep, fqt_test);
         query228.addBindValue(63);
         query228.addBindValue(63);
         query228.addBindValue(1);
     if(!query228.exec())
         qDebug() << "Query Error: " << query228.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query229 (queryPrep, fqt_test);
         query229.addBindValue(64);
         query229.addBindValue(64);
         query229.addBindValue(1);
     if(!query229.exec())
         qDebug() << "Query Error: " << query229.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query230 (queryPrep, fqt_test);
         query230.addBindValue(72);
         query230.addBindValue(72);
         query230.addBindValue(1);
     if(!query230.exec())
         qDebug() << "Query Error: " << query230.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query231 (queryPrep, fqt_test);
         query231.addBindValue(73);
         query231.addBindValue(73);
         query231.addBindValue(1);
     if(!query231.exec())
         qDebug() << "Query Error: " << query231.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query232 (queryPrep, fqt_test);
         query232.addBindValue(74);
         query232.addBindValue(74);
         query232.addBindValue(1);
     if(!query232.exec())
         qDebug() << "Query Error: " << query232.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query233 (queryPrep, fqt_test);
         query233.addBindValue(84);
         query233.addBindValue(84);
         query233.addBindValue(1);
     if(!query233.exec())
         qDebug() << "Query Error: " << query233.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query234 (queryPrep, fqt_test);
         query234.addBindValue(82);
         query234.addBindValue(82);
         query234.addBindValue(1);
     if(!query234.exec())
         qDebug() << "Query Error: " << query234.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query235 (queryPrep, fqt_test);
         query235.addBindValue(83);
         query235.addBindValue(83);
         query235.addBindValue(1);
     if(!query235.exec())
         qDebug() << "Query Error: " << query235.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query236 (queryPrep, fqt_test);
         query236.addBindValue(51);
         query236.addBindValue(51);
         query236.addBindValue(1);
     if(!query236.exec())
         qDebug() << "Query Error: " << query236.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query237 (queryPrep, fqt_test);
         query237.addBindValue(52);
         query237.addBindValue(52);
         query237.addBindValue(1);
     if(!query237.exec())
         qDebug() << "Query Error: " << query237.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query238 (queryPrep, fqt_test);
         query238.addBindValue(71);
         query238.addBindValue(71);
         query238.addBindValue(1);
     if(!query238.exec())
         qDebug() << "Query Error: " << query238.lastError();

     queryPrep = "INSERT INTO track_switch_5a VALUES (?,?,?)";
         QSqlQuery query239 (queryPrep, fqt_test);
         query239.addBindValue(81);
         query239.addBindValue(81);
         query239.addBindValue(1);
     if(!query239.exec())
         qDebug() << "Query Error: " << query239.lastError();



     queryPrep = "CREATE TABLE scheduled_train_info (id TEXT, current TEXT, destination TEXT, next TEXT, path INTEGER)";
     QSqlQuery query102 (queryPrep, fqt_test);
     if(!query102.exec())
         qDebug() << "Query Error: " << query102.lastError();

     queryPrep = "INSERT INTO scheduled_train_info VALUES (?,?,?,?,?)";
     QSqlQuery query103 (queryPrep, fqt_test);
     query103.addBindValue("1");
     query103.addBindValue("2_5");
     query103.addBindValue("2_9");
     query103.addBindValue("2_6");
     query103.addBindValue(1);
     if(!query103.exec())
         qDebug() << "Query Error: " << query103.lastError();

     queryPrep = "INSERT INTO scheduled_train_info VALUES (?,?,?,?,?)";
     QSqlQuery query104 (queryPrep, fqt_test);
     query104.addBindValue("2");
     query104.addBindValue("3_1");
     query104.addBindValue("3_14");
     query104.addBindValue("3_13");
     query104.addBindValue(2);
     if(!query104.exec())
         qDebug() << "Query Error: " << query104.lastError();

     queryPrep = "INSERT INTO scheduled_train_info VALUES ('3', '4_11', '4_14', '4_12', 3)";
     QSqlQuery query105 (queryPrep, fqt_test);
     query105.addBindValue("3");
     query105.addBindValue("4_11");
     query105.addBindValue("4_14");
     query105.addBindValue("4_12");
     query105.addBindValue(3);
     if(!query105.exec())
         qDebug() << "Query Error: " << query105.lastError();

     queryPrep = "CREATE TABLE scheduled_routes (pathid INTEGER, nextpath INTEGER, next2 TEXT,next3 TEXT,next4 TEXT,next5 TEXT,next6 TEXT,next7 TEXT,next8 TEXT,next9 TEXT,next10 TEXT,next11 TEXT)";
     QSqlQuery query106 (queryPrep, fqt_test);
     if(!query106.exec())
         qDebug() << "Query Error: " << query106.lastError();

     queryPrep = "INSERT INTO scheduled_routes VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";
     QSqlQuery query107 (queryPrep, fqt_test);
     query107.addBindValue("1");
     query107.addBindValue("NULL");
     query107.addBindValue("4_14");
     query107.addBindValue("1_16");
     query107.addBindValue("1_15");
     query107.addBindValue("1_14");
     query107.addBindValue("1_8");
     query107.addBindValue("NULL");
     query107.addBindValue("NULL");
     query107.addBindValue("NULL");
     query107.addBindValue("NULL");
     query107.addBindValue("NULL");

     if(!query107.exec())
         qDebug() << "Query Error: " << query107.lastError();

     queryPrep = "INSERT INTO scheduled_routes VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";
     QSqlQuery query108 (queryPrep, fqt_test);
     query108.addBindValue("2");
     query108.addBindValue("NULL");
     query108.addBindValue("2_1");
     query108.addBindValue("2_3");
     query108.addBindValue("NULL");
     query108.addBindValue("NULL");
     query108.addBindValue("NULL");
     query108.addBindValue("NULL");
     query108.addBindValue("NULL");
     query108.addBindValue("NULL");
     query108.addBindValue("NULL");
     query108.addBindValue("NULL");

     if(!query108.exec())
         qDebug() << "Query Error: " << query108.lastError();

     queryPrep = "INSERT INTO scheduled_routes VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";
     QSqlQuery query109 (queryPrep, fqt_test);
     query109.addBindValue("3");
     query109.addBindValue("NULL");
     query109.addBindValue("2_15");
     query109.addBindValue("2_14");
     query109.addBindValue("8_4");
     query109.addBindValue("8_3");
     query109.addBindValue("8_2");
     query109.addBindValue("NULL");
     query109.addBindValue("NULL");
     query109.addBindValue("NULL");
     query109.addBindValue("NULL");
     query109.addBindValue("NULL");
     if(!query109.exec())
         qDebug() << "Query Error: " << query109.lastError();


 }

 void MainWindow::next_test()
 {
    // Test #1 both
    QString query = "UPDATE track_power SET state = '0' WHERE ds = '1_1'";
    message("Change the status of track section with ID = '1_1' from “Active” to “Inactive”");
    QSqlQuery query2(query, fqt_test);
    query2.exec();
    message("Verify that track with ID = '1_1' is now red");

    //Test #2 both
    query = "UPDATE track_ds SET status = '1' WHERE id = '6_5'";
    message("Change the status of track section with ID = '6_5' from “Active” to “Occupied”");
    QSqlQuery query3(query, fqt_test);
    query3.exec();
    message("Verify that track with ID = '6_5' is now blue");

    // Test #3 both
    query = "UPDATE track_ds SET status = '0' WHERE id = '6_5'";
    message("Change the status of track section with ID = '6_5' from “Occupied” to “Active”");
    QSqlQuery query4(query, fqt_test);
    query4.exec();
    message("Verify that track with ID = '6_5' is now green");

    //Test #4 both
    query = "UPDATE track_ds SET status = '0' WHERE id = '2_15'";
    message("Change the status of track section with ID = '2_15' from “Occupied” to “Inactive”");
    QSqlQuery query5(query, fqt_test);
    query5.exec();
    query = "UPDATE track_power SET state = '0' WHERE ds = '2_15'";
    QSqlQuery query6(query, fqt_test);
    query6.exec();
    message("Verify that track with ID = '2_15' is now red");

    // Test #5 both
    query = "UPDATE track_power SET state = '1' WHERE ds = '3_13'";
    message("Change the status of track section with ID = '3_13' from “Inactive” to “Active”");
    QSqlQuery query7(query, fqt_test);
    query7.exec();
    message("Verify that track with ID = '3_13' is now green");

    // Test #6 both
    query = "UPDATE track_ds SET status = '1' WHERE id = '2_15'";
    message("Change the status of track section with ID = '2_15' from “Inactive” to “Occupied”");
    QSqlQuery query8(query, fqt_test);
    query8.exec();
    message("Verify that track with ID = '2_15' is now blue");

    //Test #6 both
    query = "UPDATE scheduled_train_info SET current = '1_16' WHERE path = 1";
    message("Change the location of Locomotive #1 to the track segment with ID = '1_16'");
    QSqlQuery query9(query, fqt_test);
    query = "UPDATE track_ds SET status = '1' WHERE id = '1_16'";
    query9.exec();
    QSqlQuery query10(query, fqt_test);\
    query10.exec();
    message("Verify that track with ID = '1_16' is blue and Locomotive #1 is now located on track segment with ID = '1_16'");


    //Test#7 both
    query = "UPDATE scheduled_train_info SET current = '2_3' WHERE path = 2";
    message("Change the location of Locomotive #2 to the track segment with ID = '2_3'");
    QSqlQuery query11(query, fqt_test);
    query = "UPDATE track_ds SET status = '1' WHERE id = '2_3'";
    query11.exec();
    QSqlQuery query12(query, fqt_test);\
    query12.exec();
    message("Verify that track with ID = '2_3' is blue and Locomotive #2 is now located on track segment with ID = '2_3'");

    // Test #8 both
    query = "UPDATE scheduled_train_info SET current = '2_14' WHERE path = 3";
    message("Change the location of Locomotive #3 to the track segment with ID = '2_14'");
    QSqlQuery query13(query, fqt_test);
    query = "UPDATE track_ds SET status = '1' WHERE id = '2_14'";
    query13.exec();
    QSqlQuery query14(query, fqt_test);\
    query14.exec();
    message("Verify that track with ID = '2_14' is blue and Locomotive #3 is now located on track segment with ID = '2_14'");

    // Test #9 both
    query = "UPDATE track_switch_5a SET position = '1' WHERE id = 11";
    message("Change the position of the switch with ID = '11' from 'Pass' to 'Bypass'");
    QSqlQuery query16(query, fqt_test);
    query16.exec();
    message("Verify that switch with ID = '11' is now black");

    // Test #10 database, code cant be verified
    query = "UPDATE track_switch_5a SET position = '0' WHERE id = 12";
    message("Change the position of the switch with ID = '12' from 'Bypass' to 'Pass'");
    QSqlQuery query15(query, fqt_test);
    query15.exec();
    message("Verify that switch with ID = '12' is now purple");


 }


