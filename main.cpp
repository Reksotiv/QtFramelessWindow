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
    frameless.SetContent(w);
    frameless.SetWindowTitle("Frameless Window");
    frameless.SetWindowIcon(QIcon(":/Images/icon-32.png"));

    frameless.show();
    return a.exec();
}
