/****************************************************************************
** Authors: Dario Menasce, Stefano Terzo
**
** I.N.F.N. Milan-Bicocca
** Piazza  della Scienza 3, Edificio U2
** Milano, 20126
**
****************************************************************************/
#include "rocwidget.h"
#include "xmlParser.h"
#include "ui_rocwidget.h"
#include "MessageTools.h"
#include "newNonStandardTabDialog.h"
#include "remNonStandardTabDialog.h"
#include <QMessageBox>
#include <QFileDialog>

//================================================================================
ROCWidget::ROCWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ROCWidget)
{
    ui->setupUi(this);
}

//================================================================================
ROCWidget::~ROCWidget()
{
    delete ui;
}

//================================================================================
QTabWidget * ROCWidget::getNonStandardRowTW(void)
{
 return ui->nonStandardRowTW ;
}

//================================================================================
QTabWidget * ROCWidget::getNonStandardColTW(void)
{
 return ui->nonStandardColTW ;
}

//================================================================================
void ROCWidget::initialize(xmlROC * theROC)
{
  ROC_ = theROC ;

  ss_.str(""); ss_ << ROC_->getDescription() ;
  ui->rocDescriptionLE->textIsAttribute (true                            ) ;
  ui->rocDescriptionLE->setText         ("description",ss_.str()         ) ; // Assign value from xml file
  ui->rocDescriptionLE->assignXmlElement(ROC_->getNode()                 ) ;
  ui->rocDescriptionLE->setInnerGeometry(ui->rocDescriptionLE->geometry()) ;

  lEditsDef lEdits ;

  lEdits["id"                 ] = ui->ROCIdLE;
  lEdits["MAX_ROWS"           ] = ui->rowLayoutLE;
  lEdits["MAX_COLS"           ] = ui->colLayoutLE;
  lEdits["standardRowPitch"   ] = ui->standardRowPitchLE;
  lEdits["standardColPitch"   ] = ui->standardColPitchLE;
  lEdits["orientation"        ] = ui->orientationLayoutLE;
  lEdits["calibrationFilePath"] = ui->calibFileLE;
  lEdits["pos"                ] = ui->posLE;

  int count = 0 ;
  for(lEditsDef::iterator lEi=lEdits.begin(); lEi!=lEdits.end(); ++lEi)
  {
    if(lEi->first == "id" || lEi->first == "pos" )
    {
      ((customLineEdit*)lEi->second)->textIsAttribute(true  ) ;
      ((customLineEdit*)lEi->second)->needsReturn    (      ) ;
    }
    else
    {
      ((customLineEdit*)lEi->second)->textIsAttribute(false ) ;
    }

    if(lEi->first == "MAX_ROWS" || lEi->first == "MAX_COLS" || lEi->first == "pos")
    {
      ((customLineEdit*)lEi->second)->setRange      (-1000,
                                                      1000 ) ;
    }
    else if(lEi->first == "calibrationFilePath")
    {
      ((customLineEdit*)lEi->second)->setRange      (".*") ;
    }
    else
    {
      ((customLineEdit*)lEi->second)->setRange      (-1000.,
                                                      1000.,
                                                      4    ) ;

    }
    ((customLineEdit*)lEi->second)->setText         (lEi->first,
                                                     boost::algorithm::trim_right_copy(
                                                     boost::algorithm::trim_left_copy (
                                                     ROC_->keyValue_[lEi->first].toStdString()))) ;
    ((customLineEdit*)lEi->second)->assignXmlElement(theROC->getNode()                         ) ;
    ((customLineEdit*)lEi->second)->setInnerGeometry(((customLineEdit*)lEi->second)->geometry()) ;

    count++ ;
  }


  ui->ROCInUseCB->setChecked(ROC_->isUsed());
  ui->ROCDataGB ->setEnabled(ROC_->isUsed()) ;
  ui->ROCInUseCB->setEnabled(true) ;

  connect(ui->ROCInUseCB, SIGNAL(isToggled (bool)),
          this,           SLOT  (ROCToggled(bool))) ;

  ui->rocDescriptionLE->setInnerGeometry(ui->rocDescriptionLE->geometry()) ;
}

//================================================================================
void ROCWidget::ROCToggled(bool inUse)
{
  ss_.str("") ; ss_ << "Toggling ROC " << ROC_->keyValue_["id"].toInt() << " to " << inUse ;
  ui->ROCDataGB->setEnabled(inUse) ;
  ui->ROCInUseCB->setEnabled(true) ;

  ROC_->setUsed(inUse);
}

//================================================================================
void ROCWidget::on_addRowTabPB_clicked()
{
  newNonStandardTabDialog dialog(this) ;
  dialog.setType("row") ;
  dialog.setRange(0,ROC_->keyValue_["MAX_ROWS"].toInt()) ;
  dialog.exec() ;
  if( !dialog.state() ) return ;

  emit addNewTab(this,
                 ROC_,
                 QString("row"),
                 dialog.getNewRowCol(),
                 dialog.getNewPitch()) ;
}

//================================================================================
void ROCWidget::on_addColTabPB_clicked()
{
  newNonStandardTabDialog dialog(this) ;
  dialog.setType("col") ;
  dialog.setRange(0,ROC_->keyValue_["MAX_COLS"].toInt()) ;
  dialog.exec() ;
  if( !dialog.state() ) return ;

  emit addNewTab(this,
                 ROC_,
                 QString("col"),
                 dialog.getNewRowCol(),
                 dialog.getNewPitch()) ;

}

//================================================================================
void ROCWidget::on_remRowTabPB_clicked()
{
  std::vector<QString> tabLabels ;

  for(int i=0; i<ui->nonStandardRowTW->count(); ++i)
  {
    tabLabels.push_back(ui->nonStandardRowTW->tabText(i)) ;
  }

  remNonStandardTabDialog dialog(this) ;
  dialog.setType("row") ;
  dialog.setRange(tabLabels) ;
  dialog.exec() ;

  if( !dialog.state() ) return ;

  ROC_->removeNonStandardPitch("row",dialog.getSelectedValue()) ; // Remove from DOM

  ui->nonStandardRowTW->removeTab(dialog.getSelectedIndex()) ;    // Remove tab

}

//================================================================================
void ROCWidget::on_remColTabPB_clicked()
{
  std::vector<QString> tabLabels ;

  for(int i=0; i<ui->nonStandardColTW->count(); ++i)
  {
    tabLabels.push_back(ui->nonStandardColTW->tabText(i)) ;
  }

  remNonStandardTabDialog dialog(this) ;
  dialog.setType("col") ;
  dialog.setRange(tabLabels) ;
  dialog.exec() ;

  if( !dialog.state() ) return ;

  ROC_->removeNonStandardPitch("col",dialog.getSelectedValue()) ; // Remove from DOM

  ui->nonStandardColTW->removeTab(dialog.getSelectedIndex()) ;    // Remove tab

}

//================================================================================
void ROCWidget::on_browsePB_clicked()
{
  QString path     = getenv("Monicelli_CalSample_Dir");
  QString fileName = QFileDialog::getOpenFileName(this,"Calibration files",path,"text files (*.txt)");
  if (fileName.isEmpty())  return ;

  fileName = fileName.remove(path+"/");
  ui->calibFileLE->setText(fileName) ;

}
