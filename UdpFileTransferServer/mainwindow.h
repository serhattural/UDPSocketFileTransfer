#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "NetworkReceiverThread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_Image_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void showMessage(const QString &clientIp, const QString &message);
    void showImage(const QString &clientIp, const QByteArray &buffer);
    void displayError(const QString &message);

private:
    Ui::MainWindow *ui;
    NetworkReceiverThread *networkReceiverThread;
};
#endif // MAINWINDOW_H
