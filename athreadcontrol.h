#ifndef ATHREADCONTROL_H
#define ATHREADCONTROL_H

#define _THREAD_COMPLETED "completed 100"

#include <QObject>
#include <QThread>
#include <QLineEdit>
#include <QMainWindow>

class ThreadProcess;
class AThreadControl: public QObject
{
    Q_OBJECT
public:
    explicit AThreadControl (QLineEdit* lineEdit,QObject* parent=nullptr,void (*InWork)(ThreadProcess* threadProcess)=nullptr);
    explicit AThreadControl (QLineEdit* lineEdit,QObject* parent=nullptr,void (QObject::*ObjInWork)(ThreadProcess* threadProcess)=nullptr);
    AThreadControl (AThreadControl&)=delete;
    AThreadControl (AThreadControl&&)=delete;
    void operator= (AThreadControl&)=delete;
    void operator= (AThreadControl&&)=delete;
    ~AThreadControl ();
    void InitThread(void (AThreadControl::*Init)());
public slots:
    void ShowProgress();
    void ThreadFinished ();
signals:
    void startThreadSignal();
private:
    ThreadProcess *threadWrap;
    QThread* thread;
    bool Obj; // флаг говорящий является ли переданный процесс методом класса QObject
    QLineEdit* output_text;
    QObject* pparent;
    void (*workProcess)(ThreadProcess* threadProcess);
    void (QObject::*objWorkProcess)(ThreadProcess* threadProcess);
public:
private slots:
};

class ThreadProcess: public QObject
{
    Q_OBJECT
public:
    explicit ThreadProcess (void (*InWork)(ThreadProcess* threadProcess), QObject* parent);
    explicit ThreadProcess (void (QObject::*ObjInWork)(ThreadProcess* threadProcess), QObject* parent);
    ThreadProcess(ThreadProcess&)=delete;
    ThreadProcess(ThreadProcess&&)=delete;
    void operator=(ThreadProcess&)=delete;
    void operator=(ThreadProcess&&)=delete;
    ~ThreadProcess ();
    void CurrentProgress(unsigned long long value, unsigned long long max);
public slots:
    void Run();
signals:
    void finished();
    void progress();
private:
    void (*workProcess)(ThreadProcess* threadProcess);
    void (QObject::*objWorkProcess)(ThreadProcess* threadProcess);
    QObject* pparent;
    bool Obj;// флаг говорящий является ли переданный процесс методом класса QObject
public:
    int percentage;
};

#endif // ATHREADCONTROL_H
