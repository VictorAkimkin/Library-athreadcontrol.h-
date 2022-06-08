#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "athreadcontrol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void someVoidFunction(ThreadProcess* threadProcess);
signals:

private slots:
   void on_pushButton_clicked();
   void on_lineEdit_textChanged(const QString &arg1);

   void on_pushButton_2_clicked();

public slots:
private:
    Ui::MainWindow *ui;
    AThreadControl *threadControl;
};

void someVoidFunction(ThreadProcess* threadProcess); //  тестовая нагрузка для потока

#endif // MAINWINDOW_H
