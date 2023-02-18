#include "dialogtesttimer.h"
#include "ui_dialogtesttimer.h"

DialogTestTimer::DialogTestTimer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTestTimer)
{
    ui->setupUi(this);
    ui->timeEdit->setTime(QTime(0, 0, 0, 0));

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &DialogTestTimer::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &DialogTestTimer::reject);

    setWindowTitle("Конфигурация Теста");
    show();
}

DialogTestTimer::~DialogTestTimer()
{
    delete ui;
}

void DialogTestTimer::accept()
{
    if (!ui->checkBox->isChecked())
        ui->timeEdit->setTime(QTime(0, 0, 0, 0));
    emit timeRemaining(ui->timeEdit->time());
    deleteLater();
}


void DialogTestTimer::reject()
{
    deleteLater();
}

