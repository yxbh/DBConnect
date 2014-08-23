#include "AboutDialog.hpp"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget * p_pParent) :
    QDialog(p_pParent),
    m_pUi(new Ui::AboutDialog)
{
    m_pUi->setupUi(this);

    // disable resizing.
    this->setFixedSize(this->size());
}

AboutDialog::~AboutDialog(void)
{
    delete m_pUi;
}
