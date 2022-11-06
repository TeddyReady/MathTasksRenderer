#include "generatorwindow.h"
#include "ui_generatorwindow.h"

GeneratorWindow::GeneratorWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::GeneratorWindow)
{
    ui->setupUi(this);
    ui->tasksList->addItem(eulerFunction.taskName);
}

GeneratorWindow::~GeneratorWindow()
{
    delete ui;
}


/*void GeneratorWindow::on_tasksList_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug() << "Hey";
}*/
