#include "mainwindow.h"

#include <QApplication>

#include "framelesswindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FramelessWindow frameless;


    MainWindow* w = new MainWindow(&frameless); // ??? parent
    frameless.SetContent(w);

    frameless.show();
    return a.exec();
}
