#include "DBConnectMainWindow.hpp"
#include "ui_DBConnectMainWindow.h"
#include <QDebug>
#include <QSplitterHandle>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#include "AboutDialog.hpp"
#include "Config.hpp"

DBConnectMainWindow::DBConnectMainWindow(QWidget * parent)
    : QMainWindow(parent)
    , ui(new Ui::DBConnectMainWindow)
{
    this->ui->setupUi(this);
    this->readSettings();


    //// auto fit cell to content (DISABLED DUE TO PERFORMANCE HIT).
    //ui->tableView_QueryResult->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->tableView_QueryResult->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // connect model to table view.
    ui->tableView_QueryResult->setModel(&fullFetchModel);

    // set flags
    isInStreamMode = ui->checkBox_Stream->isChecked();
}

DBConnectMainWindow::~DBConnectMainWindow(void)
{
    delete ui;
}

void DBConnectMainWindow::readSettings(void)
{
    this->move(Config::getMainWindowPosition());
    this->resize(Config::getMainWindowSize());
}

void DBConnectMainWindow::writeSettings(void)
{
    Config::saveMainWindowPosition(this->pos());
    Config::saveMainWindowSize(this->size());
}

void DBConnectMainWindow::disconnect(void)
{
    // clear query associated with DB.
    queryModel.setQuery(QSqlQuery());

    // clear model and close database.
    queryModel.clear();
    fullFetchModel.clear();
    dbConnection.close();

    // remove the connection.
    QString connection_name(dbConnection.connectionName());
    dbConnection = QSqlDatabase();
    dbConnection.removeDatabase(connection_name);
}

void DBConnectMainWindow::connect_via_odbc(void)
{
    dbConnection = QSqlDatabase::addDatabase("QODBC3");
    if (!dbConnection.isValid())
    {
        ui->statusbar->showMessage("Invalid driver type: QODBC3");
        return;
    }

    dbConnection.setUserName(ui->lineEdit_DBUsername->text());
    dbConnection.setPassword(ui->lineEdit_DBUserpassword->text());
    QString connection_option;
    connection_option += "Driver={" + ui->lineEdit_DBDriver->text() + "};"
            "Protocol=" + ui->lineEdit_DBProtocol->text() + ";"
            "Hostname=" + ui->lineEdit_DBHostName->text() + ";"
            "Port=" + ui->lineEdit_DBPort->text() + ";"
            "DATABASE=" + ui->lineEdit_DBDatabase->text() + ";";
    dbConnection.setDatabaseName(connection_option);

    dbConnection.open();
}

void DBConnectMainWindow::connect_to_db2(void)
{
    dbConnection = QSqlDatabase::addDatabase("QODBC3");
    if (!dbConnection.isValid()) return;

    dbConnection.setConnectOptions("SQL_ATTR_ODBC_VERSION = SQL_OV_ODBC3");
    dbConnection.setUserName(ui->lineEdit_DBUsername->text());
    dbConnection.setPassword(ui->lineEdit_DBUserpassword->text());

    //// with QODBC3
    QString connection_option;
    connection_option += "Driver={" + ui->lineEdit_DBDriver->text() + "};"
            "Protocol=" + ui->lineEdit_DBProtocol->text() + ";"
            "Hostname=" + ui->lineEdit_DBHostName->text() + ";"
            "Port=" + ui->lineEdit_DBPort->text() + ";"
            "DATABASE=" + ui->lineEdit_DBDatabase->text() + ";";
    dbConnection.setDatabaseName(connection_option);

    //// with QDB2
//    dbConnection.setHostName(ui->lineEdit_DBHostName->text());
//    dbConnection.setDatabaseName(ui->lineEdit_DBDatabase->text());
//    dbConnection.setPort(ui->lineEdit_DBPort->text().toInt());

    dbConnection.open();
}

void DBConnectMainWindow::connect_to_sqlite3(void)
{
    dbConnection = QSqlDatabase::addDatabase("QSQLITE");
    if (!dbConnection.isValid()) return;

    dbConnection.setUserName(ui->lineEdit_DBUsername->text());
    dbConnection.setPassword(ui->lineEdit_DBUserpassword->text());
    dbConnection.setDatabaseName(ui->lineEdit_DBDatabase->text());

    dbConnection.open();
}

void DBConnectMainWindow::connect_to_sqlite2(void)
{
    dbConnection = QSqlDatabase::addDatabase("QSQLITE2");
    if (!dbConnection.isValid()) return;

    dbConnection.setUserName(ui->lineEdit_DBUsername->text());
    dbConnection.setPassword(ui->lineEdit_DBUserpassword->text());
    dbConnection.setDatabaseName(ui->lineEdit_DBDatabase->text());

    dbConnection.open();
}

void DBConnectMainWindow::connect_to_mysql(void)
{
    dbConnection = QSqlDatabase::addDatabase("QMySQL");
    if (!dbConnection.isValid()) return;

    dbConnection.setHostName(ui->lineEdit_DBHostName->text());
    dbConnection.setPort(ui->lineEdit_DBPort->text().toInt());
    dbConnection.setUserName(ui->lineEdit_DBUsername->text());
    dbConnection.setPassword(ui->lineEdit_DBUserpassword->text());
    dbConnection.setDatabaseName(ui->lineEdit_DBDatabase->text());

    dbConnection.open();
}

void DBConnectMainWindow::connect_generic(void)
{
    // select driver type.
    dbConnection = QSqlDatabase::addDatabase(ui->comboBox_DBDriverType->currentText());
    if (!dbConnection.isValid()) return;

    // set connection parameters.
    dbConnection.setHostName(ui->lineEdit_DBHostName->text());
    dbConnection.setPort(ui->lineEdit_DBPort->text().toInt());
    dbConnection.setDatabaseName(ui->lineEdit_DBDatabase->text());
    dbConnection.setUserName(ui->lineEdit_DBUsername->text());
    dbConnection.setPassword(ui->lineEdit_DBUserpassword->text());

    // connect.
    dbConnection.open();
}

void DBConnectMainWindow::connect_via_fastodbc(void)
{
    dbConnection = QSqlDatabase::addDatabase("FastODBC");
    if (!dbConnection.isValid()) return;

    dbConnection.setUserName(ui->lineEdit_DBUsername->text());
    dbConnection.setPassword(ui->lineEdit_DBUserpassword->text());

    //// with FastODBC, similar to QODBC3
    QString connection_option;
    connection_option += "Driver={" + ui->lineEdit_DBDriver->text() + "};"
            "Protocol=" + ui->lineEdit_DBProtocol->text() + ";"
            "Hostname=" + ui->lineEdit_DBHostName->text() + ";"
            "Port=" + ui->lineEdit_DBPort->text() + ";"
            "DATABASE=" + ui->lineEdit_DBDatabase->text() + ";";
    dbConnection.setDatabaseName(connection_option);

    dbConnection.open();
}

void DBConnectMainWindow::hideConnectionPanel(void)
{
    auto * const wksp_splitter = ui->splitter_ConnectionAndWorkspace;
    auto sizes = wksp_splitter->sizes();
    sizes[0] = 0; // connection panel is 1st widget.
    wksp_splitter->setSizes(sizes);
}

void DBConnectMainWindow::showConnectionPanel(void)
{
    auto * const wksp_splitter = ui->splitter_ConnectionAndWorkspace;
    auto sizes = wksp_splitter->sizes();
    sizes[0] = wksp_splitter->widget(0)->maximumHeight(); // connection panel is 1st widget.
    wksp_splitter->setSizes(sizes);
}

void DBConnectMainWindow::closeEvent(QCloseEvent * event)
{
    this->writeSettings();

    this->QMainWindow::closeEvent(event);
}

void DBConnectMainWindow::on_pushButton_ConnectDB_clicked(void)
{
    // close previous connection and create new one.
    this->disconnect();

    // set new connection parameters.
    const QString & requested_db_type(ui->comboBox_DBDriverType->currentText());
    if (requested_db_type == "ODBC3" || requested_db_type == "ODBC")
        this->connect_via_odbc();
    else if (requested_db_type == "DB2")
        this->connect_to_db2();
    else if (requested_db_type == "SQLITE3")
        this->connect_to_sqlite3();
    else if (requested_db_type == "SQLITE2")
        this->connect_to_sqlite2();
    else if (requested_db_type == "FastODBC")
        this->connect_via_fastodbc();
    else
        this->connect_generic();

    // check for connection error.
    if (!dbConnection.isValid())
    {
        ui->statusbar->showMessage("Invalid driver type: " + ui->comboBox_DBDriverType->currentText());
    }
    else if (dbConnection.isOpenError() || !dbConnection.isOpen())
    {
        QString db_error_msg(dbConnection.lastError().text().trimmed());
        if (db_error_msg.length() > 0)
            ui->plainTextEdit_Result->setPlainText(db_error_msg);
        else
            ui->plainTextEdit_Result->setPlainText("Oh no! There're no error message...");
        ui->statusbar->showMessage("Failure to connect!");
    }
    else
    {
        this->hideConnectionPanel();
        ui->plainTextEdit_Result->clear();
        ui->statusbar->showMessage("Database connection successful!", 3000);
    }
}

void DBConnectMainWindow::on_pushButton_SubmitQuery_clicked(void)
{
    // clear current result
    ui->pushButton_ClearResult->click();

    // check DB connection valid and open.
    if (!dbConnection.isValid() || !dbConnection.isOpen())
    {
        ui->statusbar->showMessage("Database invalid or not connected!");
        return;
    }

    // prepare query and execute.
    ui->statusbar->showMessage("Submitting query...");
    QSqlQuery query(dbConnection);
    if (!isInStreamMode) query.setForwardOnly(true);
    query.prepare(ui->plainTextEdit_SQLQuery->toPlainText());
    query.exec();

    // check execution result.
    if (!query.isActive())
    {
        ui->plainTextEdit_Result->setPlainText("query failure!");
        ui->plainTextEdit_Result->appendPlainText("SQL: " + ui->plainTextEdit_SQLQuery->toPlainText());
        ui->plainTextEdit_Result->appendPlainText("Error: " + query.lastError().text());
        ui->statusbar->showMessage("Error executing query!");
        return;
    }
    else
    {
        ui->plainTextEdit_Result->clear();
        ui->statusbar->showMessage("Query execution sucessful!");
    }

    // fill in differentmodel depending on fetch mode.
    ui->statusbar->showMessage("Fetching result data...");
    if (!isInStreamMode)
    {
        int row = 0;
        // fill in header
        for (int field = 0; field < query.record().count(); ++field)
            fullFetchModel.setHorizontalHeaderItem(field, new QStandardItem(query.record().fieldName(field)));

        // fill in table
        while (query.next())
        {
            //ui->statusbar->showMessage("Fetching row " + QString::number(row+1));
            for (int field = 0; field < query.record().count(); ++field)
                fullFetchModel.setItem(row, field, new QStandardItem(query.value(field).toString().trimmed()));
            ++row;

            // process application events.
            //QApplication::processEvents();
        }

        ui->tableView_QueryResult->resizeColumnsToContents();

        ui->statusbar->showMessage("Fetch completed!", 3000);
    }
    else
    {
        queryModel.setQuery(query);
        while (queryModel.canFetchMore())
        {
            queryModel.fetchMore();

            // process application events.
            //QApplication::processEvents();
        }
    }
}

void DBConnectMainWindow::on_action_About_triggered(void)
{
    (new AboutDialog(this))->show();
}

void DBConnectMainWindow::on_action_SubmitSQL_triggered(void)
{
    ui->pushButton_SubmitQuery->click();
}

void DBConnectMainWindow::on_action_Quit_triggered(void)
{
    this->close();
}

void DBConnectMainWindow::on_comboBox_DBDriverType_currentTextChanged(const QString & p_rDbType)
{
    const QString db_type = p_rDbType.toUpper();

    ui->lineEdit_DBProtocol->setText("TCPIP");

    if (db_type == "DB2")
    {
        ui->lineEdit_DBDriver->setText("IBM DB2 ODBC DRIVER");
    }
    else if (db_type == "SQLITE3" ||
             db_type == "SQLITE2" ||
             db_type == "MySQL")
    {
        ui->lineEdit_DBDriver->setText("");
        ui->lineEdit_DBHostName->setText("");
        ui->lineEdit_DBDatabase->setText("");
        ui->lineEdit_DBProtocol->setText("");
        ui->lineEdit_DBPort->setText("");
    }
    else
    {
        ui->lineEdit_DBDriver->setText("");
        ui->lineEdit_DBHostName->setText("");
        ui->lineEdit_DBDatabase->setText("");
        ui->lineEdit_DBProtocol->setText("");
        ui->lineEdit_DBPort->setText("");
    }
}

void DBConnectMainWindow::on_pushButton_DisconnectDB_clicked(void)
{
    this->disconnect();
    ui->statusbar->showMessage("Disconnected!", 2000);
}

void DBConnectMainWindow::on_pushButton_ClearSQLStatement_clicked(void)
{
    ui->plainTextEdit_SQLQuery->clear();
}

void DBConnectMainWindow::on_lineEdit_DBUserpassword_returnPressed(void)
{
    ui->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBUsername_returnPressed(void)
{
    ui->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBProtocol_returnPressed(void)
{
    ui->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBPort_returnPressed(void)
{
    ui->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBHostName_returnPressed(void)
{
    ui->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBDatabase_returnPressed(void)
{
    ui->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBDriver_returnPressed(void)
{
    ui->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_pushButton_ClearResult_clicked(void)
{
    queryModel.setQuery(QSqlQuery());
    fullFetchModel.clear();
}

void DBConnectMainWindow::on_checkBox_Stream_toggled(bool p_Checked)
{
    isInStreamMode = p_Checked;
    if (isInStreamMode)
        ui->tableView_QueryResult->setModel(&queryModel);
    else
        ui->tableView_QueryResult->setModel(&fullFetchModel);
}

void DBConnectMainWindow::on_action_ShowConnectionPanel_triggered(void)
{
    auto * const wksp_splitter = ui->splitter_ConnectionAndWorkspace;
    wksp_splitter->sizes()[0] == 0 ? this->showConnectionPanel() : this->hideConnectionPanel();
}

void DBConnectMainWindow::on_action_AboutQt_triggered(void)
{
    qApp->aboutQt();
}
