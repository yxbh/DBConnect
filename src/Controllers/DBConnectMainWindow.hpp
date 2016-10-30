#ifndef DBCONNECT_HPP
#define DBCONNECT_HPP

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QStandardItemModel>

namespace Ui
{
class DBConnectMainWindow;
}

/**
 * \class DBConnectMainWindow
 * \brief Controller & logic for the application.
*/
class DBConnectMainWindow
    : public QMainWindow
{
    Q_OBJECT

private:
    Ui::DBConnectMainWindow * m_pUi;

    QSqlDatabase        m_DbConnection;
    QSqlQueryModel      m_QueryModel;
    QStandardItemModel  m_FullFetchModel;

    bool                m_Stream;

public:
    explicit DBConnectMainWindow(QWidget * p_pParent = nullptr);
    ~DBConnectMainWindow(void);

private:
    /** Disconnect the current database connection. */
    void disconnect(void);

    /** Connect to a database using ODBC. */
    void connect_via_odbc(void);
    /** Connect to a DB2 database. */
    void connect_to_db2(void);
    /** Connect to a SQLITE3 database. */
    void connect_to_sqlite3(void);
    /** Connect to a SQLITE2 database. */
    void connect_to_sqlite2(void);
    /** Connect to a MYSQL database. */
    void connect_to_mysql(void);
    /** Last resort if the above cant be used. Will likely fail. */
    void connect_generic(void);
    /** Connect to a database using the custom FastODBC driver layer. */
    void connect_via_fastodbc(void);

    /** Hide the connection panel. */
    void hideConnectionPanel(void);
    /** Show the connection panel. */
    void showConnectionPanel(void);

private slots:
    void on_pushButton_ConnectDB_clicked(void);
    void on_pushButton_SubmitQuery_clicked(void);
    void on_action_About_triggered(void);
    void on_action_SubmitSQL_triggered(void);
    void on_action_Quit_triggered(void);
    void on_comboBox_DBDriverType_currentTextChanged(const QString & p_rDbType);
    void on_pushButton_DisconnectDB_clicked(void);
    void on_pushButton_ClearSQLStatement_clicked(void);
    void on_lineEdit_DBUserpassword_returnPressed(void);
    void on_lineEdit_DBUsername_returnPressed(void);
    void on_lineEdit_DBProtocol_returnPressed(void);
    void on_lineEdit_DBPort_returnPressed(void);
    void on_lineEdit_DBHostName_returnPressed(void);
    void on_lineEdit_DBDatabase_returnPressed(void);
    void on_lineEdit_DBDriver_returnPressed(void);
    void on_checkBox_Stream_toggled(bool p_Checked);
    void on_pushButton_ClearResult_clicked(void);
    void on_action_ChangeLogs_triggered(void);
    void on_action_ShowConnectionPanel_triggered(void);
}; // DBConnectMainWindow class

#endif // DBCONNECT_HPP
