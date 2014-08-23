#ifndef CHANGELOGDIALOG_HPP
#define CHANGELOGDIALOG_HPP

#include <QDialog>

namespace Ui
{
class ChangeLogDialog;
}

/**
 * \class ChangeLogDialog
 * \brief A change dialog window that shows the change log for DBConnectMainWindowMainWindow.
*/
class ChangeLogDialog
    : public QDialog
{
    Q_OBJECT

private:
    Ui::ChangeLogDialog * m_pUi;

public:
    explicit ChangeLogDialog(QWidget * p_pParent = nullptr);
    ~ChangeLogDialog(void);
};

#endif // CHANGELOGDIALOG_HPP
