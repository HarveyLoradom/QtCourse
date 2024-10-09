#include "mainwindow.h"
#include "qforeach.h"
#include "ui_mainwindow.h"
#include"math.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitBTNs={{Qt::Key_0,ui->btnNum0},
             {Qt::Key_1,ui->btnNum1},
             {Qt::Key_2,ui->btnNum2},
             {Qt::Key_3,ui->btnNum3},
             {Qt::Key_4,ui->btnNum4},
             {Qt::Key_5,ui->btnNum5},
             {Qt::Key_6,ui->btnNum6},
             {Qt::Key_7,ui->btnNum7},
             {Qt::Key_8,ui->btnNum8},
             {Qt::Key_9,ui->btnNum9},
            };

    foreach(auto btn ,digitBTNs)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));

    digitBinarys={{Qt::Key_Asterisk,ui->btnMulti},
                  {Qt::Key_Slash,ui->btnDivide},
                  {Qt::Key_Plus,ui->btnAdd},
                  {Qt::Key_Minus,ui->btnSub},
                 };

    foreach(auto btn,digitBinarys)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    // connect(ui->btnMulti,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    // connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    // connect(ui->btnSub,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    // connect(ui->btnAdd,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));

    digitUnBinarys={{Qt::Key_Exclam,ui->btnF},  //键盘的！
                    {Qt::Key_At,ui->btnSqrt},  //键盘的@
                    {Qt::Key_AsciiCircum,ui->btnSquare}, //键盘^ 大写模式按下shift+数字键盘6
                    {Qt::Key_Percent,ui->btnPercent},    //键盘的%
                   };

    foreach(auto btn,digitUnBinarys)
        connect(btn,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    // connect(ui->btnF,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    // connect(ui->btnSqrt,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    // connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    // connect(ui->btnPercent,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));

    digitElse={{Qt::Key_Period,ui->btnDian},
               {Qt::Key_Return,ui->btnC},            //字母键盘的Enter键
               {Qt::Key_Escape,ui->btnCE},        //Esc键
               {Qt::Key_Equal,ui->btnAddSub},     // =键
               {Qt::Key_Enter,ui->btnAll},        //数字键盘Enter
               {Qt::Key_Backspace,ui->btnDelete}, //Backspace键
              };
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::btnNumClicked()
{
    QString digit=qobject_cast<QPushButton*>(sender())->text();

    if(digit=="0"&&operand=="0")
        digit="";

    if(digit!="0"&&operand=="0")
        operand="";

    operand+=digit;

    ui->Display->setText(operand);
    //ui->statusbar->showMessage( qobject_cast<QPushButton*>(sender())->text() + "btn clicked");
}



void MainWindow::on_btnDian_clicked()
{
    if(!operand.contains("."))
        operand +=  qobject_cast<QPushButton*>(sender())->text();
      ui->Display->setText(operand);
}


void MainWindow::on_btnDelete_clicked()
{
    operand=operand.left(operand.length()-1);
    ui->Display->setText(operand);
}

void MainWindow::on_btnC_clicked()
{
    if(operands.size()==0){
        operands.push_back(operand);
    }
    if(operands.size()>0){
        operands.clear();
        opcodes.clear();
        operand="";
        ui->Display->setText(operand);
    }
}

void MainWindow::on_btnCE_clicked()
{
    if(operands.size()==1&&opcodes.size()==1){
        operand="";
        ui->Display->setText(operand);
    }
    else if(operands.size()==0||operands.size()==1){
        operands.clear();
        operand="";
        ui->Display->setText(operand);
    }

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
   foreach(auto btnKey,digitBTNs.keys()){
        if(event->key() == btnKey)
            digitBTNs[btnKey]->animateClick();
   }
   foreach(auto BinarybtnKey,digitBinarys.keys()){
       if(event->key() == BinarybtnKey)
            digitBinarys[BinarybtnKey]->animateClick();
   }
   foreach(auto UnBinarybtnKey,digitUnBinarys.keys()){
       if(event->key() == UnBinarybtnKey)
           digitUnBinarys[UnBinarybtnKey]->animateClick();
   }
   foreach(auto ElsebtnKey,digitElse.keys()){
       if(event->key() == ElsebtnKey){
           digitElse[ElsebtnKey]->animateClick();
       }
   }
}

QString MainWindow::calculation(bool *ok)
{
    double result=0;
    QString first=operands.front();
    if(operands.size()==2&&opcodes.size()>0){
        //取操作数
        double operand1=operands.front().toDouble();
        operands.pop_front();
        double operand2=operands.front().toDouble();
        operands.pop_front();

        //取操作符
        QString op=opcodes.front();
        opcodes.pop_front();

        if(op=="+"){
            result=operand1+operand2;
        }
        else if(op=="-"){
            result=operand1-operand2;
        }
        else if(op=="×"){
            result=operand1*operand2;
        }
        else if(op=="÷"){
            result=operand1/operand2;
        }

        operands.push_back(QString::number(result));
        return QString::number(result);
        ui->statusbar->showMessage(QString("calculation is in progress operand is %1,opcode is %2").arg(operands.size()).arg(opcodes.size()));
    }
    else{
        ui->statusbar->showMessage(QString("operand is %1,opcode is %2").arg(operands.size()).arg(opcodes.size()));
    }

    return first;
}

void MainWindow::btnBinaryOperatorClicked()
{
    QString opcode=qobject_cast<QPushButton*>(sender())->text();
    if(operand!=""){

        operands.push_back(operand);

        operand="";

        opcodes.push_back(opcode);
        QString result=calculation();
        ui->Display->setText(result);

    }
    else if(operand==""&&operands.size()==1&&opcodes.size()==0){
        opcodes.push_back(opcode);
        QString result=calculation();
        ui->Display->setText(result);
    }
}

void MainWindow::btnUnaryOperatorClicked()
{
    if(operands.size()==0)
        operands.push_back(operand);
    if(operands.size()>0){
        QString oper=operands.front();
        operands.pop_front();
        double result=oper.toDouble();
        operand="";
        QString op=qobject_cast<QPushButton*>(sender())->text();
        if(op=="%")
            result/=100.0;
        else if(op == "1/x")
            result=1/result;
        else if(op=="x²")
            result*=result;
        else if(op=="²√x")
            result=sqrt(result);
        ui->Display->setText(QString::number(result));
        operands.push_back(QString::number(result));
    }
}

void MainWindow::on_btnAll_clicked()
{
    if(operand!=""){
        operands.push_back(operand);
        operand="";
        QString result=calculation();
        ui->Display->setText(result);
    }
}


void MainWindow::on_btnAddSub_clicked()
{
    if(operands.size()==0&&operand!=""){
        operands.push_back(operand);
    }
    if(operands.size()>0){
        QString oper=operands.front();
        operands.pop_front();
        if(oper.contains("-")){
            double result=operand.toDouble();
            ui->Display->setText(QString::number(result));
            operands.push_back(QString::number(result));
        }
        else{
            double result=operand.toDouble();
            result=0-result;
            ui->Display->setText(QString::number(result));
            operands.push_back(QString::number(result));
        }
    }
}

