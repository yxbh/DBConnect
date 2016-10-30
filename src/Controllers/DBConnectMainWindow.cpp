#include "DBConnectMainWindow.hpp"
#include "ui_DBConnectMainWindow.h"

#include "AboutDialog.hpp"
#include "ChangeLogDialog.hpp"

#include <QDebug>
#include <QSplitterHandle>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>


DBConnectMainWindow::DBConnectMainWindow(QWidget * p_pParent)
    : QMainWindow(p_pParent)
    , m_pUi(new Ui::DBConnectMainWindow)
{
    m_pUi->setupUi(this);

    //// auto fit cell to content (DISABLED DUE TO PERFORMANCE HIT).
    //m_pUi->tableView_QueryResult->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //m_pUi->tableView_QueryResult->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    // connect model to table view.
    m_pUi->tableView_QueryResult->setModel(&m_FullFetchModel);

    // set flags
    m_Stream = m_pUi->checkBox_Stream->isChecked();
}

DBConnectMainWindow::~DBConnectMainWindow(void)
{
    delete m_pUi;
}

void DBConnectMainWindow::disconnect(void)
{
    // clear query associated with DB.
    m_QueryModel.setQuery(QSqlQuery());

    // clear model and close database.
    m_QueryModel.clear();
    m_FullFetchModel.clear();
    m_DbConnection.close();

    // remove the connection.
    QString connection_name(m_DbConnection.connectionName());
    m_DbConnection = QSqlDatabase();
    m_DbConnection.removeDatabase(connection_name);
}

void DBConnectMainWindow::connect_via_odbc(void)
{
    m_DbConnection = QSqlDatabase::addDatabase("QODBC3");
    if (!m_DbConnection.isValid())
    {
        m_pUi->statusbar->showMessage("Invalid driver type: QODBC3");
        return;
    }

    m_DbConnection.setUserName(m_pUi->lineEdit_DBUsername->text());
    m_DbConnection.setPassword(m_pUi->lineEdit_DBUserpassword->text());
    QString connection_option;
    connection_option += "Driver={" + m_pUi->lineEdit_DBDriver->text() + "};"
            "Protocol=" + m_pUi->lineEdit_DBProtocol->text() + ";"
            "Hostname=" + m_pUi->lineEdit_DBHostName->text() + ";"
            "Port=" + m_pUi->lineEdit_DBPort->text() + ";"
            "DATABASE=" + m_pUi->lineEdit_DBDatabase->text() + ";";
    m_DbConnection.setDatabaseName(connection_option);

    m_DbConnection.open();
}

void DBConnectMainWindow::connect_to_db2(void)
{
    m_DbConnection = QSqlDatabase::addDatabase("QODBC3");
    if (!m_DbConnection.isValid()) return;

    m_DbConnection.setConnectOptions("SQL_ATTR_ODBC_VERSION = SQL_OV_ODBC3");
    m_DbConnection.setUserName(m_pUi->lineEdit_DBUsername->text());
    m_DbConnection.setPassword(m_pUi->lineEdit_DBUserpassword->text());

    //// with QODBC3
    QString connection_option;
    connection_option += "Driver={" + m_pUi->lineEdit_DBDriver->text() + "};"
            "Protocol=" + m_pUi->lineEdit_DBProtocol->text() + ";"
            "Hostname=" + m_pUi->lineEdit_DBHostName->text() + ";"
            "Port=" + m_pUi->lineEdit_DBPort->text() + ";"
            "DATABASE=" + m_pUi->lineEdit_DBDatabase->text() + ";";
    m_DbConnection.setDatabaseName(connection_option);

    //// with QDB2
//    m_DbConnection.setHostName(m_pUi->lineEdit_DBHostName->text());
//    m_DbConnection.setDatabaseName(m_pUi->lineEdit_DBDatabase->text());
//    m_DbConnection.setPort(m_pUi->lineEdit_DBPort->text().toInt());

    m_DbConnection.open();
}

void DBConnectMainWindow::connect_to_sqlite3(void)
{
    m_DbConnection = QSqlDatabase::addDatabase("QSQLITE");
    if (!m_DbConnection.isValid()) return;

    m_DbConnection.setUserName(m_pUi->lineEdit_DBUsername->text());
    m_DbConnection.setPassword(m_pUi->lineEdit_DBUserpassword->text());
    m_DbConnection.setDatabaseName(m_pUi->lineEdit_DBDatabase->text());

    m_DbConnection.open();
}

void DBConnectMainWindow::connect_to_sqlite2(void)
{
    m_DbConnection = QSqlDatabase::addDatabase("QSQLITE2");
    if (!m_DbConnection.isValid()) return;

    m_DbConnection.setUserName(m_pUi->lineEdit_DBUsername->text());
    m_DbConnection.setPassword(m_pUi->lineEdit_DBUserpassword->text());
    m_DbConnection.setDatabaseName(m_pUi->lineEdit_DBDatabase->text());

    m_DbConnection.open();
}

void DBConnectMainWindow::connect_to_mysql(void)
{
    m_DbConnection = QSqlDatabase::addDatabase("QMySQL");
    if (!m_DbConnection.isValid()) return;

    m_DbConnection.setHostName(m_pUi->lineEdit_DBHostName->text());
    m_DbConnection.setPort(m_pUi->lineEdit_DBPort->text().toInt());
    m_DbConnection.setUserName(m_pUi->lineEdit_DBUsername->text());
    m_DbConnection.setPassword(m_pUi->lineEdit_DBUserpassword->text());
    m_DbConnection.setDatabaseName(m_pUi->lineEdit_DBDatabase->text());

    m_DbConnection.open();
}

void DBConnectMainWindow::connect_generic(void)
{
    // select driver type.
    m_DbConnection = QSqlDatabase::addDatabase(m_pUi->comboBox_DBDriverType->currentText());
    if (!m_DbConnection.isValid()) return;

    // set connection parameters.
    m_DbConnection.setHostName(m_pUi->lineEdit_DBHostName->text());
    m_DbConnection.setPort(m_pUi->lineEdit_DBPort->text().toInt());
    m_DbConnection.setDatabaseName(m_pUi->lineEdit_DBDatabase->text());
    m_DbConnection.setUserName(m_pUi->lineEdit_DBUsername->text());
    m_DbConnection.setPassword(m_pUi->lineEdit_DBUserpassword->text());

    // connect.
    m_DbConnection.open();
}

void DBConnectMainWindow::connect_via_fastodbc(void)
{
    m_DbConnection = QSqlDatabase::addDatabase("FastODBC");
    if (!m_DbConnection.isValid()) return;

    m_DbConnection.setUserName(m_pUi->lineEdit_DBUsername->text());
    m_DbConnection.setPassword(m_pUi->lineEdit_DBUserpassword->text());

    //// with FastODBC, similar to QODBC3
    QString connection_option;
    connection_option += "Driver={" + m_pUi->lineEdit_DBDriver->text() + "};"
            "Protocol=" + m_pUi->lineEdit_DBProtocol->text() + ";"
            "Hostname=" + m_pUi->lineEdit_DBHostName->text() + ";"
            "Port=" + m_pUi->lineEdit_DBPort->text() + ";"
            "DATABASE=" + m_pUi->lineEdit_DBDatabase->text() + ";";
    m_DbConnection.setDatabaseName(connection_option);

    m_DbConnection.open();
}

void DBConnectMainWindow::hideConnectionPanel(void)
{
    auto * const wksp_splitter = m_pUi->splitter_ConnectionAndWorkspace;
    auto sizes = wksp_splitter->sizes();
    sizes[0] = 0; // connection panel is 1st widget.
    wksp_splitter->setSizes(sizes);
}

void DBConnectMainWindow::showConnectionPanel(void)
{
    auto * const wksp_splitter = m_pUi->splitter_ConnectionAndWorkspace;
    auto sizes = wksp_splitter->sizes();
    sizes[0] = wksp_splitter->widget(0)->maximumHeight(); // connection panel is 1st widget.
    wksp_splitter->setSizes(sizes);
}

void DBConnectMainWindow::on_pushButton_ConnectDB_clicked(void)
{
    // close previous connection and create new one.
    this->disconnect();

    // set new connection parameters.
    const QString & requested_db_type(m_pUi->comboBox_DBDriverType->currentText());
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
    if (!m_DbConnection.isValid())
    {
        m_pUi->statusbar->showMessage("Invalid driver type: " + m_pUi->comboBox_DBDriverType->currentText());
    }
    else if (m_DbConnection.isOpenError() || !m_DbConnection.isOpen())
    {
        QString db_error_msg(m_DbConnection.lastError().text().trimmed());
        if (db_error_msg.length() > 0)
            m_pUi->plainTextEdit_Result->setPlainText(db_error_msg);
        else
            m_pUi->plainTextEdit_Result->setPlainText("Oh no! There're no error message...");
        m_pUi->statusbar->showMessage("Failure to connect!");
    }
    else
    {
        this->hideConnectionPanel();
        m_pUi->plainTextEdit_Result->clear();
        m_pUi->statusbar->showMessage("Database connection successful!", 3000);
    }
}

void DBConnectMainWindow::on_pushButton_SubmitQuery_clicked(void)
{
    // clear current result
    m_pUi->pushButton_ClearResult->click();

    // check DB connection valid and open.
    if (!m_DbConnection.isValid() || !m_DbConnection.isOpen())
    {
        m_pUi->statusbar->showMessage("Database invalid or not connected!");
        return;
    }

    // prepare query and execute.
    m_pUi->statusbar->showMessage("Submitting query...");
    QSqlQuery query(m_DbConnection);
    if (!m_Stream) query.setForwardOnly(true);
    query.prepare(m_pUi->plainTextEdit_SQLQuery->toPlainText());
    query.exec();

    // check execution result.
    if (!query.isActive())
    {
        m_pUi->plainTextEdit_Result->setPlainText("query failure!");
        m_pUi->plainTextEdit_Result->appendPlainText("SQL: " + m_pUi->plainTextEdit_SQLQuery->toPlainText());
        m_pUi->plainTextEdit_Result->appendPlainText("Error: " + query.lastError().text());
        m_pUi->statusbar->showMessage("Error executing query!");
        return;
    }
    else
    {
        m_pUi->plainTextEdit_Result->clear();
        m_pUi->statusbar->showMessage("Query execution sucessful!");
    }

    // fill in differentmodel depending on fetch mode.
    m_pUi->statusbar->showMessage("Fetching result data...");
    if (!m_Stream)
    {
        int row = 0;
        // fill in header
        for (int field = 0; field < query.record().count(); ++field)
            m_FullFetchModel.setHorizontalHeaderItem(field, new QStandardItem(query.record().fieldName(field)));

        // fill in table
        while (query.next())
        {
            //m_pUi->statusbar->showMessage("Fetching row " + QString::number(row+1));
            for (int field = 0; field < query.record().count(); ++field)
                m_FullFetchModel.setItem(row, field, new QStandardItem(query.value(field).toString().trimmed()));
            ++row;

            // process application events.
            //QApplication::processEvents();
        }

        m_pUi->tableView_QueryResult->resizeColumnsToContents();

        m_pUi->statusbar->showMessage("Fetch completed!", 3000);
    }
    else
    {
        m_QueryModel.setQuery(query);
        while (m_QueryModel.canFetchMore())
        {
            m_QueryModel.fetchMore();

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
    m_pUi->pushButton_SubmitQuery->click();
}

void DBConnectMainWindow::on_action_Quit_triggered(void)
{
    this->close();
}

void DBConnectMainWindow::on_comboBox_DBDriverType_currentTextChanged(const QString & p_rDbType)
{
    const QString db_type = p_rDbType.toUpper();

    m_pUi->lineEdit_DBProtocol->setText("TCPIP");

    if (db_type == "DB2")
    {
        m_pUi->lineEdit_DBDriver->setText("IBM DB2 ODBC DRIVER");
    }
    else if (db_type == "SQLITE3" ||
             db_type == "SQLITE2" ||
             db_type == "MySQL")
    {
        m_pUi->lineEdit_DBDriver->setText("");
        m_pUi->lineEdit_DBHostName->setText("");
        m_pUi->lineEdit_DBDatabase->setText("");
        m_pUi->lineEdit_DBProtocol->setText("");
        m_pUi->lineEdit_DBPort->setText("");
    }
    else
    {
        m_pUi->lineEdit_DBDriver->setText("");
        m_pUi->lineEdit_DBHostName->setText("");
        m_pUi->lineEdit_DBDatabase->setText("");
        m_pUi->lineEdit_DBProtocol->setText("");
        m_pUi->lineEdit_DBPort->setText("");
    }
}

void DBConnectMainWindow::on_pushButton_DisconnectDB_clicked(void)
{
    this->disconnect();
    m_pUi->statusbar->showMessage("Disconnected!", 2000);
}

void DBConnectMainWindow::on_pushButton_ClearSQLStatement_clicked(void)
{
    m_pUi->plainTextEdit_SQLQuery->clear();
}

void DBConnectMainWindow::on_lineEdit_DBUserpassword_returnPressed(void)
{
    m_pUi->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBUsername_returnPressed(void)
{
    m_pUi->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBProtocol_returnPressed(void)
{
    m_pUi->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBPort_returnPressed(void)
{
    m_pUi->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBHostName_returnPressed(void)
{
    m_pUi->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBDatabase_returnPressed(void)
{
    m_pUi->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_lineEdit_DBDriver_returnPressed(void)
{
    m_pUi->pushButton_ConnectDB->click();
}

void DBConnectMainWindow::on_pushButton_ClearResult_clicked(void)
{
    m_QueryModel.setQuery(QSqlQuery());
    m_FullFetchModel.clear();
}

void DBConnectMainWindow::on_checkBox_Stream_toggled(bool p_Checked)
{
    m_Stream = p_Checked;
    if (m_Stream)
        m_pUi->tableView_QueryResult->setModel(&m_QueryModel);
    else
        m_pUi->tableView_QueryResult->setModel(&m_FullFetchModel);
}

void DBConnectMainWindow::on_action_ChangeLogs_triggered(void)
{
    (new ChangeLogDialog(this))->show();
}

void DBConnectMainWindow::on_action_ShowConnectionPanel_triggered(void)
{
    auto * const wksp_splitter = m_pUi->splitter_ConnectionAndWorkspace;
    wksp_splitter->sizes()[0] == 0 ? this->showConnectionPanel() : this->hideConnectionPanel();
}
