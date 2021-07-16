#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);

}


MainWindow::~MainWindow()
{
    delete ui;

}



void MainWindow::on_actionNew_triggered()
{
    this->currentfile.clear();
    ui->textEdit->clear();
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename= QFileDialog:: getOpenFileName(this,
                                                    "Select XML file to Open",
                                                    "/New_XML",
                                                    "XML files (*.xml)");
    openfile(filename);

}


void MainWindow::on_actionSave_As_triggered()
{
    QString filename=QFileDialog::getSaveFileName(this,"Save As...",
                                                  "filename",
                                                  "XML files (*.xml)") ;
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Can't Save: "+file.errorString());
        return;
    }
    this->currentfile=filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_actionSave_triggered()
{
    if (currentfile=="")
    {
        on_actionSave_As_triggered();
    }
    else{
    QFile file (this->currentfile);
    if (!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this,"Warning","Can't Save: "+file.errorString());
        return;
    }
    QTextStream out (&file);
    QString text = ui->textEdit->toPlainText();
    out<< text;
    file.close();
    }
}


void MainWindow::openfile(QString filename)
{

    QFile file(filename);
    currentfile=filename;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){

        QMessageBox::warning(this,"Warning","Can't Open: "+file.errorString());
        return;
    }
    ui->textEdit->clear();
    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setPlainText(text);
    file.close();
}


void MainWindow::on_actionCompress_current_file_triggered()
{
    if(currentfile=="")
    {
        QMessageBox::warning(this,"Warning","Open file to compress it");
        on_actionOpen_triggered();
        return;
    }
    else
    {
        auto x=currentfile.split('.');
        QString newfile=x[0];
        int n;
        n=x.size();
        for (int i=1;i<n-1 ;i++ ) {
            newfile+='.'+x[i];
        }
        newfile+=".cxl";
        //qInfo()<<newfile;
        Fcompress c (currentfile.toStdString(),newfile.toStdString());
        c.compress();
        QMessageBox box;
        box.setText("File has been compressed in:\n"+newfile);
        box.exec();
    }
}


void MainWindow::on_actionopen_compressed_file_triggered()
{
    QString filename= QFileDialog:: getOpenFileName(this,
                                                    "Select XML file to Open",
                                                    "/New_XML",
                                                    "CXL files (*.cxl)");
    if (filename=="")
        return;
    auto x=filename.split('.');
    QString newfile=x[0];
    int n;
    n=x.size();
    for (int i=1;i<n-1 ;i++ ) {
        newfile+='.'+x[i];
    }
    newfile+=".xml";
    // decompress
    Fcompress c(filename.toStdString(),newfile.toStdString());
    c.decompress();
    //open extracted file
    openfile(newfile);
}


void MainWindow::on_actionBeutify_Format_triggered()
{
    std::string raw=ui->textEdit->toPlainText().toStdString();
    if (raw.size())
    {
        //raw="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"+raw;
        //qInfo()<<QString::fromStdString(raw);
        Tree t(raw);
//        std::string test=t.prettify();
//        std::std::cout<<test;
//        qInfo()<<"======================";
//        qInfo()<< QString::fromStdString(test);

        ui->textEdit->setText(QString::fromStdString(t.prettify()));
    }
    return;
}


void MainWindow::traverse(node* root)
{
    if (root->getChild().size() == 0) {
        std::cout << "===========================================================" << std::endl;
        std::cout << "NAME : " + root->tagName() << "\nATTRIBIUTE:" << root->getAttr() << std::endl;
        std::cout << "is_tag : " + root->isTag() << "\nhave_data :  " << root->getHaveData() << std::endl;
        std::cout << "DATA : " + root->data << "\nself_close:" << root->self_close << std::endl;
        std::cout << "comment : " + root->comment << "\nnum_childern:" << root->children.size() << std::endl;
        std::cout << "===========================================================" << std::endl;
        return;
    }
    std::cout << "===========================================================" << std::endl;
    std::cout << "NAME : " + root->tagName() << "\nATTRIBIUTE:" << root->getAttr() << std::endl;
    std::cout << "is_tag : " + root->isTag() << "\nhave_data :  " << root->getHaveData() << std::endl;
    std::cout << "DATA : " + root->data << "\nself_close:" << root->self_close << std::endl;
    std::cout << "comment : " + root->comment << "\nnum_childern:" << root->children.size() << std::endl;
    std::cout << "===========================================================" << std::endl;
    //std::cout << "===========================================================" << std::endl;
    //std::cout << "comment : " + root->comment <<
    //std::cout<< "\nnum_childern:" << root->children.size() << std::endl;
    //for (auto i : root->children) {

    //	std::cout << "//////" ;
    //	std::cout << i->tag_name<<std::endl;
    //	std::cout <<"is_data: "<< i->getHaveData() << std::endl;
    //	std::cout << "\\\\";
    //	std::cout << std::endl;
    //}
    //std::cout << "===========================================================" << std::endl;
    int j = root->getChild().size();
    for (int i = 0; i < j; i++)
        traverse(root->getChild()[i]);

}
void MainWindow::on_actionConvert_to_JSON_triggered()
{
    std::string raw=ui->textEdit->toPlainText().toStdString();
    if (!raw.size());
    else
    {

        Tree t(raw);


        raw=t.convert_json();
        QString notraw=QString::fromStdString(raw);
        //ui->textEdit_2->setPlainText(notraw);

        JSONviewer jsonwin;
        jsonwin.setModal(true);
        jsonwin.settext(notraw);
    }
    return;
}


void MainWindow::on_actionMinify_triggered()
{
    std::string raw=ui->textEdit->toPlainText().toStdString();
    std::string newtxt;
    if (raw.size())
    {

        //qInfo()<<QString::fromStdString(raw);
        Tree t(raw);

        newtxt=t.minify();

        ui->textEdit->setPlainText(QString::fromStdString(newtxt));

    }
    return;
}


void MainWindow::on_pushButton_2_clicked()
{
    std::string raw=ui->textEdit->toPlainText().toStdString();
    if (!raw.size());
    else
    {
        raw="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"+raw;
        //qInfo()<<QString::fromStdString(raw);
        Tree t(raw);
        //traverse(t.getRoot());

        raw=t.convert_json();
        ui->textEdit_2->setText(QString::fromStdString(raw));
        //JSONviewer jsonwin;
        //jsonwin.setModal(true);
        //jsonwin.settext(QString::fromStdString(raw));
    }
    return;
}

