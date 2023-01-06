#include "dialogresults.h"
#include "ui_dialogresults.h"

DialogResults::DialogResults(QWidget *parent, tasks_type *tasks, QVector<QString> *results) :
    QDialog(parent),
    ui(new Ui::DialogResults)
{
    ui->setupUi(this);
    userCount = 0;
    maxCount = tasks->size();
    QStandardItemModel *model = new QStandardItemModel(maxCount, 2, this);
    model->setHeaderData(0, Qt::Horizontal, "Ваш ответ");
    model->setHeaderData(1, Qt::Horizontal, "Правильный ответ");
    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);
    for (int i = 0; i < maxCount; i++) {
        model->setData(model->index(i, 0), (*results)[i]);
        model->setData(model->index(i, 1), (*tasks)[i].first.second);
        if ((*tasks)[i].second.first == SupCommands::Transposition) {
            if (TranspositionGroup((*tasks)[i].first.second, (*tasks)[i].second.second) ==
                TranspositionGroup((*results)[i], (*tasks)[i].second.second)){
                userCount++;
                model->setData(model->index(i, 0), QColor(Qt::darkGreen), Qt::BackgroundColorRole);
                model->setData(model->index(i, 1), QColor(Qt::darkGreen), Qt::BackgroundColorRole);
            } else {
                model->setData(model->index(i, 0), QColor(Qt::darkRed), Qt::BackgroundColorRole);
                model->setData(model->index(i, 1), QColor(Qt::darkRed), Qt::BackgroundColorRole);
            }
        } else {
            if ((*tasks)[i].first.second == (*results)[i]) {
                userCount++;
                model->setData(model->index(i, 0), QColor(Qt::darkGreen), Qt::BackgroundColorRole);
                model->setData(model->index(i, 1), QColor(Qt::darkGreen), Qt::BackgroundColorRole);
            } else {
                model->setData(model->index(i, 0), QColor(Qt::darkRed), Qt::BackgroundColorRole);
                model->setData(model->index(i, 1), QColor(Qt::darkRed), Qt::BackgroundColorRole);
            }
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

QRegExp getInstructions(SupCommands command)
{
    QRegExp regex;
    switch (command) {

    case SupCommands::Number:
        regex = QRegExp("-?[0-9]+");
        break;

    case SupCommands::Even:
        regex = QRegExp("(Ч|Н)");
        break;

    case SupCommands::CycleType:
        regex = QRegExp("\\[([0-9]+\\^[0-9]+(,|))+\\]");
        break;

    case SupCommands::Transposition:
        regex = QRegExp("(\\(([0-9]+(,|))+\\))+");
        break;

    } return regex;
}
