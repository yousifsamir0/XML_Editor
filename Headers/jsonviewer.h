#ifndef JSONVIEWER_H
#define JSONVIEWER_H

#include <QDialog>
#include<QTextStream>

namespace Ui {
class JSONviewer;
}

class JSONviewer : public QDialog
{
    Q_OBJECT

public:
    explicit JSONviewer(QWidget *parent = nullptr);
    void settext(QString in);
    ~JSONviewer();



private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::JSONviewer *ui;
};

#endif // JSONVIEWER_H
