/********************************************************************************
** Form generated from reading UI file 'customCheckBox.ui'
**
** Created: Fri Dec 6 10:08:22 2013
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMCHECKBOX_H
#define UI_CUSTOMCHECKBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_customCheckBox
{
public:
    QCheckBox *checkBox;

    void setupUi(QWidget *customCheckBox)
    {
        if (customCheckBox->objectName().isEmpty())
            customCheckBox->setObjectName(QString::fromUtf8("customCheckBox"));
        customCheckBox->resize(100, 22);
        checkBox = new QCheckBox(customCheckBox);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(0, 0, 100, 22));

        retranslateUi(customCheckBox);

        QMetaObject::connectSlotsByName(customCheckBox);
    } // setupUi

    void retranslateUi(QWidget *customCheckBox)
    {
        customCheckBox->setWindowTitle(QApplication::translate("customCheckBox", "customCheckBox", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class customCheckBox: public Ui_customCheckBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMCHECKBOX_H
