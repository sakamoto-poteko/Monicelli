/********************************************************************************
** Form generated from reading UI file 'customTextEdit.ui'
**
** Created: Fri Dec 6 10:08:17 2013
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMTEXTEDIT_H
#define UI_CUSTOMTEXTEDIT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_customTextEdit
{
public:
    QTextEdit *textEdit;

    void setupUi(QWidget *customTextEdit)
    {
        if (customTextEdit->objectName().isEmpty())
            customTextEdit->setObjectName(QString::fromUtf8("customTextEdit"));
        customTextEdit->resize(400, 60);
        textEdit = new QTextEdit(customTextEdit);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(4, 4, 391, 51));

        retranslateUi(customTextEdit);

        QMetaObject::connectSlotsByName(customTextEdit);
    } // setupUi

    void retranslateUi(QWidget *customTextEdit)
    {
        customTextEdit->setWindowTitle(QApplication::translate("customTextEdit", "customTextEdit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class customTextEdit: public Ui_customTextEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMTEXTEDIT_H
