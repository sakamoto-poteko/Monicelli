/********************************************************************************
** Form generated from reading UI file 'customLineEdit.ui'
**
** Created: Fri Dec 6 10:08:14 2013
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMLINEEDIT_H
#define UI_CUSTOMLINEEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_customLineEdit
{
public:
    QLineEdit *lineEdit;

    void setupUi(QWidget *customLineEdit)
    {
        if (customLineEdit->objectName().isEmpty())
            customLineEdit->setObjectName(QString::fromUtf8("customLineEdit"));
        customLineEdit->resize(90, 22);
        lineEdit = new QLineEdit(customLineEdit);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(0, 0, 90, 22));

        retranslateUi(customLineEdit);

        QMetaObject::connectSlotsByName(customLineEdit);
    } // setupUi

    void retranslateUi(QWidget *customLineEdit)
    {
        customLineEdit->setWindowTitle(QApplication::translate("customLineEdit", "customLineEdit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class customLineEdit: public Ui_customLineEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMLINEEDIT_H
