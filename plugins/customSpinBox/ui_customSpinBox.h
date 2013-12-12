/********************************************************************************
** Form generated from reading UI file 'customSpinBox.ui'
**
** Created: Fri Dec 6 10:08:19 2013
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMSPINBOX_H
#define UI_CUSTOMSPINBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_customSpinBox
{
public:
    QSpinBox *spinBox;

    void setupUi(QWidget *customSpinBox)
    {
        if (customSpinBox->objectName().isEmpty())
            customSpinBox->setObjectName(QString::fromUtf8("customSpinBox"));
        customSpinBox->resize(100, 19);
        spinBox = new QSpinBox(customSpinBox);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));

        retranslateUi(customSpinBox);

        QMetaObject::connectSlotsByName(customSpinBox);
    } // setupUi

    void retranslateUi(QWidget *customSpinBox)
    {
        customSpinBox->setWindowTitle(QApplication::translate("customSpinBox", "customSpinBox", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class customSpinBox: public Ui_customSpinBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMSPINBOX_H
