библиотека "athreadcontrol.h"
включает 2 класса AThreadControl и ThreadProcess

используя библиотеку в своей программе необходимо создать вспомогательный виджет типа QlineEdit
и сделать его невидимым в конструкторе вашего класса (ui->lineEdit->setVisible(false);)
у этого виджета следует отслеживать значение события textChanged для контроля состояния потока

для запуска процесса необходимо создать объект класса AThreadControl передав в него два параметра
первый параметр - указатель на виджет типа QlineEdit,
второй параметр - 
либо указатель на процесс объявленый вне класса, тип void (*)(ThreadProcess*) 
либо указатель на метод класса, тип void (QObject::*)(ThreadProcess*)

примеры создания экземпляра класса:
   threadControl=new AThreadControl(ui->lineEdit,this,::someVoidFunction);
или
   typedef void (QObject::*pf)(ThreadProcess*);
    typedef void (QObject::*pf)(ThreadProcess*);
    pf ObjWorkProcess=static_cast<pf>(&MainWindow::someVoidFunction); // или pf ObjWorkProcess=(pf)&MainWindow::someVoidFunction;
    threadControl=new AThreadControl(ui->lineEdit,this,ObjWorkProcess);

примеры объявления выполняемых процессов для приведенных выше экземпляров класса AThreadControl
void someVoidFunction(ThreadProcess* threadProcess);
void MainWindow::someVoidFunction(ThreadProcess *threadProcess);

в передаваемых процессах для отслеживания прогресса можно (не обязательно) использовать метод 
threadProcess->CurrentProgress(_process_part, max_progress);
в этом случае, в событие textChanged вспомогательного виджета типа QlineEdit передается значение 
arg1 типа const QString с цифрой процента прогресса выполнения процесса (от "0" до "100") 

по окончании выполнения процесса в потоке значение arg1 события textChanged станет равным _THREAD_COMPLETED
(макрос #define _THREAD_COMPLETED "completed 100")
