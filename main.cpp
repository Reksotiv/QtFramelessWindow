#include "mainwindow.h"

#include <QApplication>

#include "framelesswindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FramelessWindow frameless;


    MainWindow* w = new MainWindow(&frameless); // ??? parent
    frameless.SetContent(w);
    frameless.SetWindowTitle("Frameless Window");
    frameless.SetWindowIcon(QIcon(":/Images/icon-32.png"));

    frameless.show();
    return a.exec();
}
