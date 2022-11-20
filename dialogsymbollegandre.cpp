#include "dialogsymbollegandre.h"
#include "ui_dialogsymbollegandre.h"

DialogSymbolLegandre::DialogSymbolLegandre(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSymbolLegandre)
{
    ui->setupUi(this);
    ui->lineMinA->setText(QString::number(-1000));
    ui->lineMaxA->setText(QString::number(1000));
    ui->lineMinP->setText(QString::number(3));
    ui->lineMaxP->setText(QString::number(1000));
    ui->btnDefault->setChecked(true);
    ui->spinDefault->setEnabled(true);
    ui->btnPrimes->setChecked(false);
    ui->spinPrimes->setDisabled(true);
    ui->btn1->setChecked(false);
    ui->spin1->setDisabled(true);
    ui->btn2->setChecked(false);
    ui->spin2->setDisabled(true);
    ui->btnNotEvenPrimes->setChecked(false);
    ui->spinNotEvenPrimes->setDisabled(true);

    count = 0;
}

DialogSymbolLegandre::~DialogSymbolLegandre()
{
    delete ui;
}

void DialogSymbolLegandre::on_buttonBox_accepted()
{
    QVector<std::pair<int, int>> tmp;
    tmp.push_back(std::make_pair(ui->lineMinA->text().toInt(), ui->lineMaxA->text().toInt()));
    tmp.push_back(std::make_pair(ui->lineMinP->text().toInt(), ui->lineMaxP->text().toInt()));
    if (ui->btnDefault->isChecked()) count += ui->spinDefault->value();
    if (ui->btnPrimes->isChecked()) count += ui->spinPrimes->value();
    if (ui->btn1->isChecked()) count += ui->spin1->value();
    if (ui->btn2->isChecked()) count += ui->spin2->value();
    if (ui->btnNotEvenPrimes->isChecked()) count += ui->spinNotEvenPrimes->value();
    emit dialogSymbolLegandreMeta(count);

    if (ui->btnDefault->isChecked())
        emit dialogSymbolLegandre(ui->spinDefault->value(), tmp[0], tmp[1], SymbolLegandreOptions::Default);
    if (ui->btnPrimes->isChecked())
        emit dialogSymbolLegandre(ui->spinPrimes->value(), tmp[0], tmp[1], SymbolLegandreOptions::Primes);
    if (ui->btn1->isChecked())
        emit dialogSymbolLegandre(ui->spin1->value(), std::make_pair(-1, 0), tmp[1], SymbolLegandreOptions::aEqual_1);
    if (ui->btnNotEvenPrimes->isChecked())
        emit dialogSymbolLegandre(ui->spinNotEvenPrimes->value(), tmp[0], tmp[1], SymbolLegandreOptions::NotEvenPrimes);
    if (ui->btn2->isChecked())
        emit dialogSymbolLegandre(ui->spin2->value(), std::make_pair(2, 3), tmp[1], SymbolLegandreOptions::aEqual2);
    close();
}

void DialogSymbolLegandre::on_buttonBox_rejected()
{
    close();
}

void DialogSymbolLegandre::on_btnDefault_clicked()
{
    if (ui->btnDefault->isChecked()) {
        ui->btnDefault->setChecked(true);
        ui->spinDefault->setEnabled(true);
    } else {
        ui->btnDefault->setChecked(false);
        ui->spinDefault->setDisabled(true);
    }
}

void DialogSymbolLegandre::on_btnPrimes_clicked()
{
    if (ui->btnPrimes->isChecked()) {
        ui->btnPrimes->setChecked(true);
        ui->spinPrimes->setEnabled(true);
    } else {
        ui->btnPrimes->setChecked(false);
        ui->spinPrimes->setDisabled(true);
    }
}

void DialogSymbolLegandre::on_btn1_clicked()
{
    if (ui->btn1->isChecked()) {
        ui->btn1->setChecked(true);
        ui->spin1->setEnabled(true);
    } else {
        ui->btn1->setChecked(false);
        ui->spin1->setDisabled(true);
    }
}

void DialogSymbolLegandre::on_btn2_clicked()
{
    if (ui->btn2->isChecked()) {
        ui->btn2->setChecked(true);
        ui->spin2->setEnabled(true);
    } else {
        ui->btn2->setChecked(false);
        ui->spin2->setDisabled(true);
    }
}

void DialogSymbolLegandre::on_btnNotEvenPrimes_clicked()
{
    if (ui->btnNotEvenPrimes->isChecked()) {
        ui->btnNotEvenPrimes->setChecked(true);
        ui->spinNotEvenPrimes->setEnabled(true);
    } else {
        ui->btnNotEvenPrimes->setChecked(false);
        ui->spinNotEvenPrimes->setDisabled(true);
    }
}
