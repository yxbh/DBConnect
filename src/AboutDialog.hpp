#ifndef ABOUTDIALOG_HPP
#define ABOUTDIALOG_HPP

#include <QDialog>

namespace Ui
{
class AboutDialog;
}

/**
 * \class AboutDialog
 * DBConnectMainWindowMainWindow's about dialog controller.
*/
class AboutDialog
    : public QDialog
{
    Q_OBJECT

private:
    Ui::AboutDialog * m_pUi;

public:
    explicit AboutDialog(QWidget * p_pParent = nullptr);
    ~AboutDialog(void);
};

#endif // ABOUTDIALOG_HPP
