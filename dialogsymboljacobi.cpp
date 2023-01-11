#include "dialogsymboljacobi.h"
#include "ui_dialogsymboljacobi.h"

DialogSymbolJacobi::DialogSymbolJacobi(QWidget *parent, bool mode) :
    QDialog(parent),
    ui(new Ui::DialogSymbolJacobi)
{
    ui->setupUi(this);
    ui->lineMinA->setText(QString::number(-1000));
    ui->lineMaxA->setText(QString::number(1000));
    ui->lineMinP->setText(QString::number(3));
    ui->lineMaxP->setText(QString::number(1000));
    ui->btnDefault->setChecked(false);
    ui->spinDefault->setDisabled(true);
    ui->btnPrimes->setChecked(false);
    ui->spinPrimes->setDisabled(true);
    ui->btn1->setChecked(false);
    ui->spin1->setDisabled(true);
    ui->btn2->setChecked(false);
    ui->spin2->setDisabled(true);
    ui->btnNotEvenPrimes->setChecked(false);
    ui->spinNotEvenPrimes->setDisabled(true);
    if (mode) {
        ui->buttonBox->button(QDialogButtonBox::Cancel)->deleteLater();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Сгенерировать задания");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon());
        isCancelExist = false;
    } else isCancelExist = true;
    count = 0;
}

DialogSymbolJacobi::~DialogSymbolJacobi()
{
    delete ui;
}

void DialogSymbolJacobi::on_buttonBox_accepted()
{
    QVector<std::pair<int, int>> tmp;
    tmp.push_back(std::make_pair(ui->lineMinA->text().toInt(), ui->lineMaxA->text().toInt() + 1));
    tmp.push_back(std::make_pair(ui->lineMinP->text().toInt(), ui->lineMaxP->text().toInt() + 1));
    if (ui->btnDefault->isChecked()) count += ui->spinDefault->value();
    if (ui->btnPrimes->isChecked()) count += ui->spinPrimes->value();
    if (ui->btn1->isChecked()) count += ui->spin1->value();
    if (ui->btn2->isChecked()) count += ui->spin2->value();
    if (ui->btnNotEvenPrimes->isChecked()) count += ui->spinNotEvenPrimes->value();
    emit dialogSymbolJacobiMeta(count); count = 0;
    if (ui->btnDefault->isChecked())
        emit dialogSymbolJacobi(ui->spinDefault->value(), tmp[0], tmp[1], SymbolJacobiOptions::Default);
    if (ui->btnPrimes->isChecked())
        emit dialogSymbolJacobi(ui->spinPrimes->value(), tmp[0], tmp[1], SymbolJacobiOptions::Primes);
    if (ui->btn1->isChecked())
        emit dialogSymbolJacobi(ui->spin1->value(), std::make_pair(-1, 0), tmp[1], SymbolJacobiOptions::aEqual_1);
    if (ui->btnNotEvenPrimes->isChecked())
        emit dialogSymbolJacobi(ui->spinNotEvenPrimes->value(), tmp[0], tmp[1], SymbolJacobiOptions::NotEvenPrimes);
    if (ui->btn2->isChecked())
        emit dialogSymbolJacobi(ui->spin2->value(), std::make_pair(2, 3), tmp[1], SymbolJacobiOptions::aEqual2);    
    if (isCancelExist) close();
}

void DialogSymbolJacobi::on_buttonBox_rejected()
{
    close();
}

void DialogSymbolJacobi::on_btnDefault_clicked()
{
    if (ui->btnDefault->isChecked()) {
        ui->btnDefault->setChecked(true);
        ui->spinDefault->setEnabled(true);
    } else {
        ui->btnDefault->setChecked(false);
        ui->spinDefault->setDisabled(true);
    }
}

void DialogSymbolJacobi::on_btnPrimes_clicked()
{
    if (ui->btnPrimes->isChecked()) {
        ui->btnPrimes->setChecked(true);
        ui->spinPrimes->setEnabled(true);
    } else {
        ui->btnPrimes->setChecked(false);
        ui->spinPrimes->setDisabled(true);
    }
}

void DialogSymbolJacobi::on_btn1_clicked()
{
    if (ui->btn1->isChecked()) {
        ui->btn1->setChecked(true);
        ui->spin1->setEnabled(true);
    } else {
        ui->btn1->setChecked(false);
        ui->spin1->setDisabled(true);
    }
}

void DialogSymbolJacobi::on_btn2_clicked()
{
    if (ui->btn2->isChecked()) {
        ui->btn2->setChecked(true);
        ui->spin2->setEnabled(true);
    } else {
        ui->btn2->setChecked(false);
        ui->spin2->setDisabled(true);
    }
}

void DialogSymbolJacobi::on_btnNotEvenPrimes_clicked()
{
    if (ui->btnNotEvenPrimes->isChecked()) {
        ui->btnNotEvenPrimes->setChecked(true);
        ui->spinNotEvenPrimes->setEnabled(true);
    } else {
        ui->btnNotEvenPrimes->setChecked(false);
        ui->spinNotEvenPrimes->setDisabled(true);
    }
}
