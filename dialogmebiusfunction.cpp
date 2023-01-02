#include "dialogmebiusfunction.h"
#include "ui_dialogmebiusfunction.h"

DialogMebiusFunction::DialogMebiusFunction(QWidget *parent, bool mode) :
    QDialog(parent),
    ui(new Ui::DialogMebiusFunction)
{
    ui->setupUi(this);
    ui->lineMin->setText(QString::number(1));
    ui->lineMax->setText(QString::number(1000));
    ui->btnDefault->setChecked(false);
    ui->spinDefault->setDisabled(true);
    ui->btnEvenPrimes->setChecked(false);
    ui->spinEvenPrimes->setDisabled(true);
    ui->btnNotEvenPrimes->setChecked(false);
    ui->spinNotEvenPrimes->setDisabled(true);
    ui->btnPrimeDegree->setChecked(false);
    ui->spinPrimeDegree->setDisabled(true);
    ui->btnPrimes->setChecked(false);
    ui->spinPrimes->setDisabled(true);
    if (mode) {
        ui->buttonBox->button(QDialogButtonBox::Cancel)->deleteLater();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Сгенерировать задания");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon());
        isCancelExist = false;
    } else isCancelExist = true;
    count = 0;
}

DialogMebiusFunction::~DialogMebiusFunction()
{
    delete ui;
}

void DialogMebiusFunction::on_buttonBox_accepted()
{
    if (ui->btnDefault->isChecked()) count += ui->spinDefault->value();
    if (ui->btnPrimes->isChecked()) count += ui->spinPrimes->value();
    if (ui->btnPrimeDegree->isChecked()) count += ui->spinPrimeDegree->value();
    if (ui->btnEvenPrimes->isChecked()) count += ui->spinEvenPrimes->value();
    if (ui->btnNotEvenPrimes->isChecked()) count += ui->spinNotEvenPrimes->value();
    emit dialogMebiusFunctionMeta(count);
    if (ui->btnDefault->isChecked())
        emit dialogMebiusFunction(ui->spinDefault->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, MebiusFunctionOptions::Default);
    if (ui->btnPrimes->isChecked())
        emit dialogMebiusFunction(ui->spinPrimes->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, MebiusFunctionOptions::Primes);
    if (ui->btnEvenPrimes->isChecked())
        emit dialogMebiusFunction(ui->spinEvenPrimes->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, MebiusFunctionOptions::EvenPrimes);
    if (ui->btnNotEvenPrimes->isChecked())
        emit dialogMebiusFunction(ui->spinNotEvenPrimes->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, MebiusFunctionOptions::NotEvenPrimes);
    if (ui->btnPrimeDegree->isChecked())
        emit dialogMebiusFunction(ui->spinPrimeDegree->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, MebiusFunctionOptions::PrimeDegree);
    if (isCancelExist) close();
}

void DialogMebiusFunction::on_buttonBox_rejected()
{
    close();
}

void DialogMebiusFunction::on_btnDefault_clicked()
{
    if (ui->btnDefault->isChecked()) {
        ui->btnDefault->setChecked(true);
        ui->spinDefault->setEnabled(true);
    } else {
        ui->btnDefault->setChecked(false);
        ui->spinDefault->setDisabled(true);
    }
}

void DialogMebiusFunction::on_btnPrimes_clicked()
{
    if (ui->btnPrimes->isChecked()) {
        ui->btnPrimes->setChecked(true);
        ui->spinPrimes->setEnabled(true);
    } else {
        ui->btnPrimes->setChecked(false);
        ui->spinPrimes->setDisabled(true);
    }
}

void DialogMebiusFunction::on_btnEvenPrimes_clicked()
{
    if (ui->btnEvenPrimes->isChecked()) {
        ui->btnEvenPrimes->setChecked(true);
        ui->spinEvenPrimes->setEnabled(true);
    } else {
        ui->btnEvenPrimes->setChecked(false);
        ui->spinEvenPrimes->setDisabled(true);
    }
}

void DialogMebiusFunction::on_btnNotEvenPrimes_clicked()
{
    if (ui->btnNotEvenPrimes->isChecked()) {
        ui->btnNotEvenPrimes->setChecked(true);
        ui->spinNotEvenPrimes->setEnabled(true);
    } else {
        ui->btnNotEvenPrimes->setChecked(false);
        ui->spinNotEvenPrimes->setDisabled(true);
    }
}

void DialogMebiusFunction::on_btnPrimeDegree_clicked()
{
    if (ui->btnPrimeDegree->isChecked()) {
        ui->btnPrimeDegree->setChecked(true);
        ui->spinPrimeDegree->setEnabled(true);
    } else {
        ui->btnPrimeDegree->setChecked(false);
        ui->spinPrimeDegree->setDisabled(true);
    }
}
