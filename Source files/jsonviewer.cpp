#include "jsonviewer.h"
#include "ui_jsonviewer.h"
#include<QFile>
#include<QString>
#include<QFileDialog>
#include<QMessageBox>



JSONviewer::JSONviewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JSONviewer)
{
    ui->setupUi(this);
}

JSONviewer::~JSONviewer()
{
    delete ui;
}

void JSONviewer::on_pushButton_clicked()
{
    QString filename=QFileDialog::getSaveFileName(this,"Save As...",
                                                  "filename",
                                                  "JSON files (*.json)") ;
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Can't Save: "+file.errorString());
        return;
    }

    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = ui->textBrowser->toPlainText();
    out << text;
    file.close();
}

void JSONviewer::settext(QString in){
    ui->textBrowser->setPlainText(in);
    this->exec();
}


void JSONviewer::on_pushButton_2_clicked()
{
    this->close();
}

