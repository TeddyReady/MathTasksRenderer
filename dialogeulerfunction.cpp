#include "dialogeulerfunction.h"
#include "ui_dialogeulerfunction.h"

DialogEulerFunction::DialogEulerFunction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEulerFunction)
{
    ui->setupUi(this);
    ui->lineMin->setText(QString::number(1));
    ui->lineMax->setText(QString::number(1000));
    ui->btnDefault->setChecked(false);
    ui->spinDefault->setDisabled(true);
    ui->btnPrime->setChecked(false);
    ui->spinPrime->setDisabled(true);
    ui->btnPrimeDegree->setChecked(false);
    ui->spinPrimeDegree->setDisabled(true);
    ui->btnMultiplyPrimes->setChecked(false);
    ui->spinMultiplyPrimes->setDisabled(true);
    ui->btnMultiplyPrimesDegree->setChecked(false);
    ui->spinMultiplyPrimesDegree->setDisabled(true);

    count = 0;
}

DialogEulerFunction::~DialogEulerFunction()
{
    delete ui;
}

void DialogEulerFunction::on_buttonBox_accepted()
{
    if (ui->btnDefault->isChecked()) count += ui->spinDefault->value();
    if (ui->btnPrime->isChecked()) count += ui->spinPrime->value();
    if (ui->btnPrimeDegree->isChecked()) count += ui->spinPrimeDegree->value();
    if (ui->btnMultiplyPrimes->isChecked()) count += ui->spinMultiplyPrimes->value();
    if (ui->btnMultiplyPrimesDegree->isChecked()) count += ui->spinMultiplyPrimesDegree->value();
    emit dialogEulerFunctionMeta(count);

    if (ui->btnDefault->isChecked())
        emit dialogEulerFunction(ui->spinDefault->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, EulerFunctionOptions::Default);
    if (ui->btnPrime->isChecked())
        emit dialogEulerFunction(ui->spinPrime->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, EulerFunctionOptions::Primes);
    if (ui->btnPrimeDegree->isChecked())
        emit dialogEulerFunction(ui->spinPrimeDegree->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, EulerFunctionOptions::PrimesDegrees);
    if (ui->btnMultiplyPrimes->isChecked())
        emit dialogEulerFunction(ui->spinMultiplyPrimes->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, EulerFunctionOptions::MultiplyPrimes);
    if (ui->btnMultiplyPrimesDegree->isChecked())
        emit dialogEulerFunction(ui->spinMultiplyPrimesDegree->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, EulerFunctionOptions::MultiplyPrimesDegrees);
    close();
}

void DialogEulerFunction::on_buttonBox_rejected()
{
    close();
}

void DialogEulerFunction::on_btnDefault_clicked()
{
    if (ui->btnDefault->isChecked()) {
        ui->btnDefault->setChecked(true);
        ui->spinDefault->setEnabled(true);
    } else {
        ui->btnDefault->setChecked(false);
        ui->spinDefault->setDisabled(true);
    }
}

void DialogEulerFunction::on_btnPrime_clicked()
{
    if (ui->btnPrime->isChecked()) {
        ui->btnPrime->setChecked(true);
        ui->spinPrime->setEnabled(true);
    } else {
        ui->btnPrime->setChecked(false);
        ui->spinPrime->setDisabled(true);
    }
}

void DialogEulerFunction::on_btnPrimeDegree_clicked()
{
    if (ui->btnPrimeDegree->isChecked()) {
        ui->btnPrimeDegree->setChecked(true);
        ui->spinPrimeDegree->setEnabled(true);
    } else {
        ui->btnPrimeDegree->setChecked(false);
        ui->spinPrimeDegree->setDisabled(true);
    }
}

void DialogEulerFunction::on_btnMultiplyPrimes_clicked()
{
    if (ui->btnMultiplyPrimes->isChecked()) {
        ui->btnMultiplyPrimes->setChecked(true);
        ui->spinMultiplyPrimes->setEnabled(true);
    } else {
        ui->btnMultiplyPrimes->setChecked(false);
        ui->spinMultiplyPrimes->setDisabled(true);
    }
}

void DialogEulerFunction::on_btnMultiplyPrimesDegree_clicked()
{
    if (ui->btnMultiplyPrimesDegree->isChecked()) {
        ui->btnMultiplyPrimesDegree->setChecked(true);
        ui->spinMultiplyPrimesDegree->setEnabled(true);
    } else {
        ui->btnMultiplyPrimesDegree->setChecked(false);
        ui->spinMultiplyPrimesDegree->setDisabled(true);
    }
}
