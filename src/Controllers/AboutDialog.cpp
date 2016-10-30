#include "AboutDialog.hpp"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->ui->setupUi(this);

    // disable resizing.
    this->setFixedSize(this->size());
}

AboutDialog::~AboutDialog(void)
{
    delete ui;
}
