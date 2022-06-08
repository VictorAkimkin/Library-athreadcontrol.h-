#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // рабочий виджет типа QlineEdit используемый потоком для передачи сингналов прогресса и окончания работы.
    // делаем невидимым
    ui->lineEdit->setVisible(false);
}
MainWindow::~MainWindow()
{
    delete ui;
}
// пример, когда в поток передается функция вне класса
void MainWindow::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);ui->pushButton_2->setEnabled(false);
    // в поток передаем указатели на рабочий виджеит QlineEdit, текущий объект и функцию типа void (ThreadProcess*)
   threadControl=new AThreadControl(ui->lineEdit,this,::someVoidFunction);

}
// пример, когда в поток передается метод класса
void MainWindow::on_pushButton_2_clicked()
{
    ui->pushButton->setEnabled(false);ui->pushButton_2->setEnabled(false);
    // в поток передаем указатели на рабочий виджеит QlineEdit, текущий объект и функцию типа void MainWindow::(ThreadProcess*)
    // приведенную к типу void QObject::(ThreadProcess*)
    typedef void (QObject::*pf)(ThreadProcess*);
    pf ObjWorkProcess=static_cast<pf>(&MainWindow::someVoidFunction);
    threadControl=new AThreadControl(ui->lineEdit,this,ObjWorkProcess);

}
// событие изменения текста переданного в поток виджета обрабатываем для отслеживания прогресса в процентах
// по окончанием работы получаем значение _THREAD_COMPLETED ("completed 100")
void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if (arg1==_THREAD_COMPLETED)
    {
        ui->label_2->setText("Процесс завершен на 100");
        ui->pushButton->setEnabled(true);ui->pushButton_2->setEnabled(true);
    }else
        ui->label_2->setText(arg1);
}
//  ---------тестовая нагрузка для потока в функции типа void MainWindow::(ThreadProcess*)---------
void MainWindow::someVoidFunction(ThreadProcess *threadProcess)
{
    int max_progress=10;
    for (int _process_part=0;_process_part<max_progress;++_process_part)
    {
       // посылает в рабочий виджет прогресс выполнения процесса (не обязателено)
       if (!(_process_part%1)) threadProcess->CurrentProgress(_process_part,max_progress);
       // непосредственно полезная  работа
       _sleep(100); //имитация нагрузки
       // ---------------
    }
}
//  -----------------------------------------------------------------------------------------------
//*************************************************************************************
//  ---------тестовая нагрузка для потока в функции типа void (ThreadProcess*)---------
void someVoidFunction(ThreadProcess* threadProcess)
{
    int max_progress=10;
    for (int _process_part=0;_process_part<max_progress;++_process_part)
    {
       // посылает в рабочий виджет прогресс выполнения процесса (не обязателено)
       if (!(_process_part%1)) threadProcess->CurrentProgress(_process_part,max_progress);
       // непосредственно полезная  работа
       _sleep(100); //имитация нагрузки
       // ---------------
    }
}
//*************************************************************************************


