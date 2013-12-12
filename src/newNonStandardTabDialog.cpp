/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/
#include "newNonStandardTabDialog.h"
#include "ui_newNonStandardTabDialog.h"
#include "MessageTools.h"
#include <QMessageBox>

//================================================================================
newNonStandardTabDialog::newNonStandardTabDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newNonStandardTabDialog)
{
    ui->setupUi(this);

    this->setModal(true) ;

    state_ = false ;
}

//================================================================================
newNonStandardTabDialog::~newNonStandardTabDialog()
{
    delete ui;
}

//================================================================================
void newNonStandardTabDialog::setType(QString type)
{
  type_  = QString("New ") + type + QString(":") ;
  ui->rowColLB->setText(type_);
}

//================================================================================
void newNonStandardTabDialog::on_okPB_clicked()
{
  state_ = true ;
  this->accept() ;
}

//================================================================================
void newNonStandardTabDialog::on_cancelPB_clicked()
{
  this->reject();
}

//================================================================================
bool newNonStandardTabDialog::state(void)
{
  return state_ ;
}

//================================================================================
int newNonStandardTabDialog::getNewRowCol(void)
{
  return ui->rowColSB->value();
}

//================================================================================
double newNonStandardTabDialog::getNewPitch(void)
{
  return ui->pitchSB->value();
}

//================================================================================
void newNonStandardTabDialog::setRange(int min, int max)
{
  min_ = min; max_ = max ;
}

//================================================================================
void newNonStandardTabDialog::on_rowColSB_valueChanged(int value)
{
  if( ui->rowColSB->value() < min_ || ui->rowColSB->value() > max_ )
  {
    ui->rowColSB->setValue(0) ;
    ss_.str("") ; ss_ << "Value "
                      << value
                      << " is out of allowed range ("
                      << min_
                      << ", "
                      << max_
                      << ")" ;
    QMessageBox msgBox(QMessageBox::Warning, tr("Warning"),
                       tr(ss_.str().c_str()), 0, this);
    msgBox.addButton(tr("Ok"    ), QMessageBox::AcceptRole);
  }
}
