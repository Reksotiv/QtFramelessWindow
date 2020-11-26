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

#include <QApplication>
#include <QFile>
#include <QTextStream>

#include "framelesswindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile f(":/styles/default.qss");
    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }

    FramelessWindow frameless;

    MainWindow* w = new MainWindow(&frameless); // ??? parent

    frameless.setContent(w);
    frameless.setWindowTitle("Frameless Window");
    frameless.setWindowIcon(QIcon(":/Images/icon-32.png"));

    frameless.show();
    return a.exec();
}
