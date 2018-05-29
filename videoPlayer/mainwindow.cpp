#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    unsigned version = avcodec_version();
    qDebug()<<version;
}

MainWindow::~MainWindow()
{
    delete ui;
}

