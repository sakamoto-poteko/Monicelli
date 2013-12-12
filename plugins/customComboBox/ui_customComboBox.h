/********************************************************************************
** Form generated from reading UI file 'customComboBox.ui'
**
** Created: Fri Dec 6 10:08:09 2013
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMCOMBOBOX_H
#define UI_CUSTOMCOMBOBOX_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_customComboBox
{
public:
    QComboBox *comboBox;

    void setupUi(QWidget *customComboBox)
    {
        if (customComboBox->objectName().isEmpty())
            customComboBox->setObjectName(QString::fromUtf8("customComboBox"));
        customComboBox->resize(90, 22);
        comboBox = new QComboBox(customComboBox);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(0, 0, 90, 22));

        retranslateUi(customComboBox);

        QMetaObject::connectSlotsByName(customComboBox);
    } // setupUi

    void retranslateUi(QWidget *customComboBox)
    {
        customComboBox->setWindowTitle(QApplication::translate("customComboBox", "customComboBox", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class customComboBox: public Ui_customComboBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMCOMBOBOX_H
