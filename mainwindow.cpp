/*
###############################################################################
#                                                                             #
#                           The MIT License                                   #
# Copyright (C) 2020 by Reksotiv (hellfire0h@gmail.com)                       #
#                https://github.com/Reksotiv                                  #
# Sources code: https://github.com/Reksotiv/QtFramelessWindow                 #
#                                                                             #
###############################################################################
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QAction>
#include "framelesswindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // думаю это херовый способ
    // QDialog
    QAction *act_about = new QAction(tr("About"), this);
    connect(act_about, &QAction::triggered, this, &MainWindow::testQDialog);
    // QDialogButtonBox
    QAction *act_dButBox = new QAction(tr("DialBut box"), this);
    connect(act_dButBox, &QAction::triggered, this, &MainWindow::testQDialogButtonBox);

    FramelessWindow *frameless_window = qobject_cast<FramelessWindow*>(parent);
    if (frameless_window){
        frameless_window->titleBarMenu()->addAction(act_about);
        frameless_window->titleBarMenu()->addAction(act_dButBox);
    } else {
        qWarning()<<"cant find object FramelessWindow in mainwindow.cpp"<<"\naction disabled";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testQDialog()
{
    //QScopedPointer<FramelessWindow> frame (new FramelessWindow(this));
    std::unique_ptr<FramelessWindow> frame (new FramelessWindow(this));

    // create test QMessageBox with FramelessWindow parent for align
    QMessageBox *msgBox = new QMessageBox(frame.get());
    msgBox->setWindowTitle("About QtFramelessWindow");
    msgBox->setIcon(QMessageBox::Question);
    QString build("<p align=\"center\">Build with Qt "+ QString(QT_VERSION_STR));
    QString source("<br> Source code available on <A HREF=\"https://github.com/Reksotiv/QtFramelessWindow\">GutHub</A>");
    msgBox->setText(build + source);
    msgBox->setInformativeText("<p align=\"center\">bla bla bla?");
    msgBox->setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Save);
    // set msgBox inside FramelessWindow
    frame->setContent(msgBox);

    msgBox->exec();
}

void MainWindow::testQDialogButtonBox()
{
    std::unique_ptr<FramelessWindow> frame (new FramelessWindow(this));
    // create test QDialogButtonBox with FramelessWindow parent for align
    QDialog dlg(frame.get());
    QVBoxLayout layout(&dlg);
    QLineEdit line;
    layout.addWidget(&line);
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), &dlg, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), &dlg, SLOT(reject()));
    layout.addWidget(buttonBox);
    dlg.setLayout(&layout);
    // set QDialogButtonBox inside FramelessWindow
    frame->setContent(&dlg);

    if(dlg.exec() == QDialog::Accepted)
    {
        qDebug()<<"dialog accept";
    }
}

void MainWindow::testQTool()
{
    std::unique_ptr<FramelessWindow> frame (new FramelessWindow(this));

}


void MainWindow::on_pushButton_default_clicked()
{
    QFile f(":/styles/default.qss");
    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}

void MainWindow::on_pushButton_white_clicked()
{
    QFile f(":qss/qss.qss");
    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}

void MainWindow::on_pushButton_dark_clicked()
{
    QFile f(":qdarkstyle/style.qss");
    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
}
