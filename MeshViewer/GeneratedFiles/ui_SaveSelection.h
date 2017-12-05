/********************************************************************************
** Form generated from reading UI file 'SaveSelection.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVESELECTION_H
#define UI_SAVESELECTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DialogSaveSelection
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QPushButton *cancelButton;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QLabel *labelHeadLine;
    QTextEdit *textEditHeadLines;
    QLineEdit *lineEditData;
    QLineEdit *lineEditPath;
    QLabel *labelDataLine;
    QLabel *labelSavePath;

    void setupUi(QDialog *DialogSaveSelection)
    {
        if (DialogSaveSelection->objectName().isEmpty())
            DialogSaveSelection->setObjectName(QStringLiteral("DialogSaveSelection"));
        DialogSaveSelection->resize(400, 300);
        layoutWidget = new QWidget(DialogSaveSelection);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 250, 351, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QStringLiteral("okButton"));

        hboxLayout->addWidget(okButton);

        labelHeadLine = new QLabel(DialogSaveSelection);
        labelHeadLine->setObjectName(QStringLiteral("labelHeadLine"));
        labelHeadLine->setGeometry(QRect(28, 60, 54, 20));
        textEditHeadLines = new QTextEdit(DialogSaveSelection);
        textEditHeadLines->setObjectName(QStringLiteral("textEditHeadLines"));
        textEditHeadLines->setGeometry(QRect(110, 30, 261, 101));
        lineEditData = new QLineEdit(DialogSaveSelection);
        lineEditData->setObjectName(QStringLiteral("lineEditData"));
        lineEditData->setGeometry(QRect(110, 150, 261, 20));
        lineEditPath = new QLineEdit(DialogSaveSelection);
        lineEditPath->setObjectName(QStringLiteral("lineEditPath"));
        lineEditPath->setGeometry(QRect(110, 191, 261, 20));
        labelDataLine = new QLabel(DialogSaveSelection);
        labelDataLine->setObjectName(QStringLiteral("labelDataLine"));
        labelDataLine->setGeometry(QRect(28, 155, 54, 12));
        labelSavePath = new QLabel(DialogSaveSelection);
        labelSavePath->setObjectName(QStringLiteral("labelSavePath"));
        labelSavePath->setGeometry(QRect(28, 195, 54, 12));

        retranslateUi(DialogSaveSelection);
        QObject::connect(okButton, SIGNAL(clicked()), DialogSaveSelection, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), DialogSaveSelection, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogSaveSelection);
    } // setupUi

    void retranslateUi(QDialog *DialogSaveSelection)
    {
        DialogSaveSelection->setWindowTitle(QApplication::translate("DialogSaveSelection", "Dialog", 0));
        cancelButton->setText(QApplication::translate("DialogSaveSelection", "Cancel", 0));
        okButton->setText(QApplication::translate("DialogSaveSelection", "Save", 0));
        labelHeadLine->setText(QApplication::translate("DialogSaveSelection", "Head Line", 0));
        labelDataLine->setText(QApplication::translate("DialogSaveSelection", "Data Line", 0));
        labelSavePath->setText(QApplication::translate("DialogSaveSelection", "Save Path", 0));
    } // retranslateUi

};

namespace Ui {
    class DialogSaveSelection: public Ui_DialogSaveSelection {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVESELECTION_H
