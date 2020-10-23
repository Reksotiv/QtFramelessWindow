#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->label->setText("Source code available on <a href=\"http://example.com/\">Click Here!</a>");
    ui->label->setTextFormat(Qt::RichText);
    //ui->label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->label->setOpenExternalLinks(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

