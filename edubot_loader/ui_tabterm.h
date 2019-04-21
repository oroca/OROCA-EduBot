/********************************************************************************
** Form generated from reading UI file 'tabterm.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABTERM_H
#define UI_TABTERM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TabTerm
{
public:
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QGridLayout *gridLayout;
    QPushButton *pushButton_Update;
    QComboBox *comboBox_FileList;
    QPushButton *Load;
    QComboBox *comboBox_PortList;
    QWidget *widget_2;
    QVBoxLayout *verticalLayout;

    void setupUi(QWidget *TabTerm)
    {
        if (TabTerm->objectName().isEmpty())
            TabTerm->setObjectName(QString::fromUtf8("TabTerm"));
        TabTerm->resize(640, 480);
        verticalLayout_2 = new QVBoxLayout(TabTerm);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        groupBox = new QGroupBox(TabTerm);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy);
        groupBox->setMinimumSize(QSize(250, 100));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton_Update = new QPushButton(groupBox);
        pushButton_Update->setObjectName(QString::fromUtf8("pushButton_Update"));

        gridLayout->addWidget(pushButton_Update, 0, 1, 1, 1);

        comboBox_FileList = new QComboBox(groupBox);
        comboBox_FileList->setObjectName(QString::fromUtf8("comboBox_FileList"));

        gridLayout->addWidget(comboBox_FileList, 1, 0, 1, 1);

        Load = new QPushButton(groupBox);
        Load->setObjectName(QString::fromUtf8("Load"));

        gridLayout->addWidget(Load, 1, 1, 1, 1);

        comboBox_PortList = new QComboBox(groupBox);
        comboBox_PortList->setObjectName(QString::fromUtf8("comboBox_PortList"));
        comboBox_PortList->setMinimumSize(QSize(200, 0));

        gridLayout->addWidget(comboBox_PortList, 0, 0, 1, 1);


        formLayout->setLayout(0, QFormLayout::LabelRole, gridLayout);


        horizontalLayout->addWidget(groupBox);

        widget_2 = new QWidget(TabTerm);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));

        horizontalLayout->addWidget(widget_2);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(TabTerm);

        QMetaObject::connectSlotsByName(TabTerm);
    } // setupUi

    void retranslateUi(QWidget *TabTerm)
    {
        TabTerm->setWindowTitle(QApplication::translate("TabTerm", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("TabTerm", "Setup", nullptr));
        pushButton_Update->setText(QApplication::translate("TabTerm", "Update", nullptr));
        Load->setText(QApplication::translate("TabTerm", "Load", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TabTerm: public Ui_TabTerm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABTERM_H
