#include "athreadcontrol.h"
//#include "ui_mainwindow.h"

//---------------------ThreadControl-----------------------------------------
// в конструктор передается функция вне класса
AThreadControl::AThreadControl(QLineEdit* lineEdit, QObject* parent, void (*InWork)(ThreadProcess* threadProcess))
    :output_text(lineEdit),pparent(parent),workProcess(InWork),Obj(false)
{
    InitThread (&AThreadControl::startThreadSignal);// Инициализация процесса и потока
}
// в конструктор передается метод класса
AThreadControl::AThreadControl(QLineEdit *lineEdit, QObject *parent, void (QObject::*ObjInWork)(ThreadProcess *))
    :output_text(lineEdit),pparent(parent),objWorkProcess(ObjInWork),Obj(true)
{
    InitThread (&AThreadControl::startThreadSignal);// Инициализация процесса и потока
}
AThreadControl::~AThreadControl()
{
}
// Инициализация процесса и потока
void AThreadControl::InitThread(void (AThreadControl::*start)())
{
    output_text->setText("0");
    thread= new QThread (this);// создание потока
    if (Obj) threadWrap = new ThreadProcess(objWorkProcess,pparent);
        else threadWrap = new ThreadProcess(workProcess,pparent);// создание процесса
    threadWrap->moveToThread(thread);// помещение процесса в поток
    connect(this,SIGNAL(startThreadSignal()),threadWrap,SLOT(Run()));// сигнал запуска процесса в потоке
    connect(this,SIGNAL(destroyed()),threadWrap,SLOT(quit()));// если объект AThreadControl уничтожается, поток корректно завершается
    connect(thread, &QThread::finished, threadWrap, &QObject::deleteLater);// если поток завершается, то экземпляр процесса удаляется
    connect(threadWrap,SIGNAL(progress()),this,SLOT(ShowProgress()));// сигнал отслеживания прогресса выполнения процесса
    connect(threadWrap,SIGNAL(finished()),this,SLOT(ThreadFinished()));// сигнал о том, что процесс выполнен

    thread->start();// запуск потока
    emit (this->*start)();// запуск процесса
}
// передача в служебный виджет информации о прогрессе выполнения процесса
void AThreadControl::ShowProgress()
{
      output_text->setText(QString::number(threadWrap->percentage));
}
// завершение потока
void AThreadControl::ThreadFinished()
{
    output_text->setText(_THREAD_COMPLETED);// передача информации об окончании процесса
    if (thread) {thread->quit(); thread->deleteLater();}
    this->deleteLater();
}
//---------------------ThreadControl end----------------------------------
//---------------------ThreadWrap-----------------------------------------
ThreadProcess::ThreadProcess(void (*InWork)(ThreadProcess* threadProcess), QObject* parent)
    :workProcess(InWork),percentage(0),Obj(false),pparent (parent)
{
}

ThreadProcess::ThreadProcess(void (QObject::*ObjInWork)(ThreadProcess *), QObject* parent)
    :objWorkProcess(ObjInWork),percentage(0),Obj(true),pparent (parent)
{
}

ThreadProcess::~ThreadProcess()
{
}
// расчет прогресса выполнения процесса в процентах
void ThreadProcess::CurrentProgress(unsigned long long value, unsigned long long max)
{
  percentage=value*100/max; emit progress();
}
// запуск процесса
void ThreadProcess::Run()
{
    if (Obj) (pparent->*objWorkProcess)(this);
    else workProcess(this);
   emit finished();
}
//---------------------ThreadWrap end-------------------------------------

