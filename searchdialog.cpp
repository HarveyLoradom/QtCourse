#include "searchdialog.h"
#include "ui_searchdialog.h"
#include<QMessageBox>

SearchDialog::SearchDialog(QWidget *parent,QPlainTextEdit *textEdit)
    : QDialog(parent)
    , ui(new Ui::SearchDialog)
{
    ui->setupUi(this);

    pTextEdit=textEdit;
    ui->rbdown->setChecked(true);
}

SearchDialog::~SearchDialog()
{
    delete ui;
}

void SearchDialog::on_Findnext_clicked()
{
    QString target=ui->searchEdit->text();

    if(target==""||pTextEdit==nullptr)
        return;

    QString text=pTextEdit->toPlainText();
    QTextCursor c=pTextEdit->textCursor();
    int index=-1;

    if(ui->rbdown->isChecked()){
        index=text.indexOf(target,c.position(),
                             ui->BigSmall->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);

        if(index>=0){
            c.setPosition(index);
            c.setPosition(index+target.length(),QTextCursor::KeepAnchor);
            pTextEdit->setTextCursor(c);
        }
    }
    else if(ui->rbup->isChecked()){
        index=text.lastIndexOf(target,c.position()-text.length()-1,ui->BigSmall->isChecked()?Qt::CaseSensitive:Qt::CaseInsensitive);

        if(index>=0){
            c.setPosition(index+target.length());
            c.setPosition(index,QTextCursor::KeepAnchor);
            pTextEdit->setTextCursor(c);
        }
    }

    if(index<0){
        QMessageBox msg(this);
        msg.setWindowTitle("再试一遍");
        msg.setText(QString("找不到")+target);
        msg.setWindowFlags(Qt::Drawer);
        msg.setIcon(QMessageBox::Information);
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
    }

}


void SearchDialog::on_cancel_clicked()
{
    accept();
}

