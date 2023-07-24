#include "dialogresults.h"
#include "ui_dialogresults.h"

DialogResults::DialogResults(const tasks_type &tasks, const QVector<QString> &results, QWidget *parent) :
    QDialog(parent), ui(new Ui::DialogResults)
{
    ui->setupUi(this);
    userCount = 0;
    maxCount = tasks.size();
    QStandardItemModel *model = new QStandardItemModel(maxCount, 2, this);
    model->setHeaderData(0, Qt::Horizontal, "Ваш ответ");
    model->setHeaderData(1, Qt::Horizontal, "Правильный ответ");
    ui->tableView->setModel(model);
    ui->tableView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableView->setColumnWidth(0, (this->width() / 2));
    ui->tableView->setColumnWidth(1, (this->width() / 2));

//    for (int i = 0; i < maxCount; i++) {
//        model->setData(model->index(i, 0), results[i]);
//        model->setData(model->index(i, 1), std::get<1>(tasks[i]));
//        if (std::get<2>(tasks[i]) == SupCommands::Transposition) {
//            if (TranspositionGroup(std::get<1>(tasks[i]), std::get<3>(tasks[i])) ==
//                TranspositionGroup(results[i], std::get<3>(tasks[i]))){
//                userCount++;
//                model->setData(model->index(i, 0), QColor(Qt::darkGreen), Qt::BackgroundColorRole);
//                model->setData(model->index(i, 1), QColor(Qt::darkGreen), Qt::BackgroundColorRole);
//            } else {
//                model->setData(model->index(i, 0), QColor(Qt::darkRed), Qt::BackgroundColorRole);
//                model->setData(model->index(i, 1), QColor(Qt::darkRed), Qt::BackgroundColorRole);
//            }
//        } else if (std::get<2>(tasks[i]) == SupCommands::MultiTransposition) {
//            if (TranspositionGroup(std::get<1>(tasks[i]), std::get<3>(tasks[i])).simplify(std::get<3>(tasks[i])) ==
//                TranspositionGroup(results[i], std::get<3>(tasks[i])).simplify(std::get<3>(tasks[i]))){
//                userCount++;
//                model->setData(model->index(i, 0), QColor(Qt::darkGreen), Qt::BackgroundColorRole);
//                model->setData(model->index(i, 1), QColor(Qt::darkGreen), Qt::BackgroundColorRole);
//            } else {
//                model->setData(model->index(i, 0), QColor(Qt::darkRed), Qt::BackgroundColorRole);
//                model->setData(model->index(i, 1), QColor(Qt::darkRed), Qt::BackgroundColorRole);
//            }
//        } else {
//            if (std::get<1>(tasks[i]) == results[i]) {
//                userCount++;
//                model->setData(model->index(i, 0), QColor(Qt::darkGreen), Qt::BackgroundColorRole);
//                model->setData(model->index(i, 1), QColor(Qt::darkGreen), Qt::BackgroundColorRole);
//            } else {
//                model->setData(model->index(i, 0), QColor(Qt::darkRed), Qt::BackgroundColorRole);
//                model->setData(model->index(i, 1), QColor(Qt::darkRed), Qt::BackgroundColorRole);
//            }
//        }
//    }
    ui->userResult->setText(QString::number(userCount));
    ui->maxResult->setText(QString::number(maxCount));
    ui->progressBar->setValue(static_cast<int>((static_cast<double>(userCount) / static_cast<double>(maxCount)) * 100));

    if (ui->progressBar->value() == 100)
        ui->commentLine->setText("Так держать, превосходный результат!");
    else if (ui->progressBar->value() >= 80)
        ui->commentLine->setText("Отличный результат!");
    else if (ui->progressBar->value() >= 60)
        ui->commentLine->setText("Хороший результат, однако есть чему учиться!");
    else if (ui->progressBar->value() >= 40)
        ui->commentLine->setText("Нууу... Можно было и получше");
    else if (ui->progressBar->value() >= 20)
        ui->commentLine->setText("Кажется стоило бы изучить теорию...");
    else
        ui->commentLine->setText("Учиться, учиться и еще раз учиться!");
}

DialogResults::~DialogResults()
{
    delete ui;
}

QRegExp getInstructions(SupCommands command)
{
    switch (command) {
    case SupCommands::Number:
        return QRegExp("-?[0-9]+");

    case SupCommands::Even:
        return QRegExp("(Ч|Н)");

    case SupCommands::CycleType:
        return QRegExp("\\[([0-9]+\\^[0-9]+(,|))+\\]");

    case SupCommands::Transposition:
        return QRegExp("(\\(([0-9]+(,|))+\\))+");

    case SupCommands::MultiTransposition:
        return QRegExp("(\\(([0-9]+(,|))+\\))+");

    case SupCommands::Name:
        return QRegExp("[А-Яа-я]+");
    } return QRegExp("");
}
