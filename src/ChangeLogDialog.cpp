#include "ChangeLogDialog.hpp"
#include "ui_ChangeLogDialog.h"

ChangeLogDialog::ChangeLogDialog(QWidget * p_pParent) :
    QDialog(p_pParent),
    m_pUi(new Ui::ChangeLogDialog)
{
    m_pUi->setupUi(this);
}

ChangeLogDialog::~ChangeLogDialog(void)
{
    delete m_pUi;
}
