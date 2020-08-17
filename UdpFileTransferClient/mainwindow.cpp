#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <NetworkSender.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionSelectAndSend_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.jpg *.jpeg)");
    QPixmap pixmap(fileName);

    ui->labelImage->setPixmap(pixmap);

    NetworkSender *sender = new NetworkSender();

    std::string message = "Bu bir test mesajıdır...";
    //sender->SendMessage(message);
    sender->SendFile(fileName.toStdString());
}

