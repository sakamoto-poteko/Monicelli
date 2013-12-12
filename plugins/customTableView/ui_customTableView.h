/********************************************************************************
** Form generated from reading UI file 'customTableView.ui'
**
** Created: Fri Dec 6 10:08:24 2013
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CUSTOMTABLEVIEW_H
#define UI_CUSTOMTABLEVIEW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QTableView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_customTableView
{
public:
    QTableView *tableView;

    void setupUi(QWidget *customTableView)
    {
        if (customTableView->objectName().isEmpty())
            customTableView->setObjectName(QString::fromUtf8("customTableView"));
        customTableView->resize(935, 60);
        tableView = new QTableView(customTableView);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setGeometry(QRect(860, 18, 67, 25));

        retranslateUi(customTableView);

        QMetaObject::connectSlotsByName(customTableView);
    } // setupUi

    void retranslateUi(QWidget *customTableView)
    {
        customTableView->setWindowTitle(QApplication::translate("customTableView", "customTableView", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class customTableView: public Ui_customTableView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CUSTOMTABLEVIEW_H
