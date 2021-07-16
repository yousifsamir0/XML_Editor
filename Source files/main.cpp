#include "mainwindow.h"
#include<fcompress.h>
#include <QApplication>
#include<QCoreApplication>
#include<QTextStream>
#include<huffman.h>
#include<Tree.h>

//void wr_test(){

//   // "D:/wtext.txt"
////    Fcompress file1("rtext.txt","D:/wtext.cxml");
////    file1.compress();
////    Fcompress file2("D:/wtext.cxml","D:/decompressed.xml");
////    file2.decompress();

//    QFile file("D:/note.xml");
//    file.open(QFile::ReadOnly | QFile::Text);
//    QString s =file.readAll();
//    Tree t(s.toStdString());
//    qInfo()<<QString::fromStdString(t.prettify());



//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //wr_test();
    MainWindow w;
    w.show();
    return a.exec();
}
