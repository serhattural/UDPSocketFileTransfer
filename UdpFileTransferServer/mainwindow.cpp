#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QObject>
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QMessageBox>
#include "NetworkReceiverThread.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    networkReceiverThread = new NetworkReceiverThread(this);
    connect(networkReceiverThread, &NetworkReceiverThread::newMessage, this, &MainWindow::showMessage);
    connect(networkReceiverThread, &NetworkReceiverThread::newFile, this, &MainWindow::showImage);
    connect(networkReceiverThread, &NetworkReceiverThread::newError, this, &MainWindow::displayError);
    //NetworkReceiver *receiver = new NetworkReceiver();
    //receiver->ReceiveMessage();
    networkReceiverThread->start();
}

MainWindow::~MainWindow()
{
    delete networkReceiverThread;
    delete ui;
}

void MainWindow::showMessage(const QString &clientIp, const QString &message)
{
    QWidget* widget = new QWidget();
    QHBoxLayout* hLayout = new QHBoxLayout(widget);
    QLabel *label = new QLabel(widget);

    label->setAutoFillBackground(true);
    label->setFrameShape(QFrame::Box);
    label->setText(message);
    label->setScaledContents(true);
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //widget->setLayout(hLayout);
    hLayout->addWidget(label);
    ui->tabWidget->addTab(widget, clientIp);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void MainWindow::showImage(const QString &clientIp, const QByteArray &buffer)
{
    QWidget* widget = new QWidget();
    QHBoxLayout* hLayout = new QHBoxLayout(widget);
    QLabel *label = new QLabel(widget);
    QPixmap *pixmap = new QPixmap();
    pixmap->loadFromData(buffer);

    label->setAutoFillBackground(true);
    label->setFrameShape(QFrame::Box);
    label->setPixmap(*pixmap);
    label->setScaledContents(true);
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //widget->setLayout(hLayout);
    hLayout->addWidget(label);
    ui->tabWidget->addTab(widget, clientIp);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void MainWindow::displayError(const QString &message)
{
    QMessageBox::information(this, "UDP File Transfer Server", message);
}

void MainWindow::on_actionOpen_Image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Image Files (*.jpg *.jpeg)");
    QWidget* widget = new QWidget();
    QHBoxLayout* hLayout = new QHBoxLayout(widget);
    QLabel *label = new QLabel(widget);
    QPixmap pix(fileName);

    label->setAutoFillBackground(true);
    label->setFrameShape(QFrame::Box);
    label->setPixmap(pix);
    label->setScaledContents(true);
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    //widget->setLayout(hLayout);
    hLayout->addWidget(label);
    ui->tabWidget->addTab(widget, fileName);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->removeTab(index);
}
