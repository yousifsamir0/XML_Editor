#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <jsonviewer.h>
#include <QFileDialog>
#include<QTextStream>
#include <QFile>
#include<QString>
#include <QMessageBox>
#include<fcompress.h>
#include<Tree.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openfile(QString filename);
    void traverse(node* root);

private slots:
    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    void on_actionSave_As_triggered();

    void on_actionSave_triggered();

    void on_actionCompress_current_file_triggered();

    void on_actionopen_compressed_file_triggered();

    void on_actionBeutify_Format_triggered();

    void on_actionConvert_to_JSON_triggered();

    void on_actionMinify_triggered();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QString currentfile="";



};
#endif // MAINWINDOW_H
