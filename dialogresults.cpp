#include "dialogresults.h"
#include "ui_dialogresults.h"

DialogResults::DialogResults(QWidget *parent, tasks_type *tasks, QVector<QString> *results) :
    QDialog(parent),
    ui(new Ui::DialogResults)
{
    setWindowTitle("Результаты теста");
    ui->setupUi(this);
    userCount = 0;
    maxCount = tasks->size();
    for (int i = 0; i < maxCount; i++) {
        //qDebug() << (*tasks)[i].second << " " << (*results)[i];
        if ((*tasks)[i].second == (*results)[i]) {
            userCount++;
        }
    }
    ui->userResult->setText(QString::number(userCount));
    ui->maxResult->setText(QString::number(maxCount));
    ui->progressBar->setValue(static_cast<int>((static_cast<double>(userCount) / static_cast<double>(maxCount)) * 100));
    if (ui->progressBar->value() == 100) {
        ui->commentLine->setText("Так держать, превосходный результат!");
    } else if (ui->progressBar->value() > 80) {
        ui->commentLine->setText("Отличный результат!");
    } else if (ui->progressBar->value() > 60) {
        ui->commentLine->setText("Хороший результат, однако есть чему учиться!");
    } else if (ui->progressBar->value() > 40) {
        ui->commentLine->setText("Нууу... Можно было и получше");
    } else if (ui->progressBar->value() > 20) {
        ui->commentLine->setText("Кажется стоило бы изучить теорию...");
    } else if (ui->progressBar->value() < 20) {
        ui->commentLine->setText("Учиться, учиться и еще раз учиться!");
    }
}

DialogResults::~DialogResults()
{
    delete ui;
}
