#include "dialogtranspositiongroup.h"
#include "ui_dialogtranspositiongroup.h"

DialogTranspositionGroup::DialogTranspositionGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTranspositionGroup)
{
    ui->setupUi(this);
    ui->spinNmin->setValue(2);
    ui->spinNmax->setValue(10);
    ui->btnWrite->setChecked(false);
    ui->spinWrite->setDisabled(true);
    ui->btnMultiply->setChecked(false);
    ui->spinMultiply->setDisabled(true);
    ui->btnInverse->setChecked(false);
    ui->spinInverse->setDisabled(true);
    ui->btnCycle->setChecked(false);
    ui->spinCycle->setDisabled(true);
    ui->btnMod2->setChecked(false);
    ui->spinMod2->setDisabled(true);
    ui->btnOrder->setChecked(false);
    ui->spinOrder->setDisabled(true);
    ui->btnCount->setChecked(false);
    ui->spinCount->setDisabled(true);

    count = 0;
}

DialogTranspositionGroup::~DialogTranspositionGroup()
{
    delete ui;
}

void DialogTranspositionGroup::on_buttonBox_accepted()
{
    if (ui->btnWrite->isChecked()) count += ui->spinWrite->value();
    if (ui->btnCycle->isChecked()) count += ui->spinCycle->value();
    if (ui->btnMultiply->isChecked()) count += ui->spinMultiply->value();
    if (ui->btnInverse->isChecked()) count += ui->spinInverse->value();
    if (ui->btnMod2->isChecked()) count += ui->spinMod2->value();
    if (ui->btnCount->isChecked()) count += ui->spinCount->value();
    if (ui->btnOrder->isChecked()) count += ui->spinOrder->value();
    emit dialogTranspositionGroupMeta(count);

    if (ui->btnWrite->isChecked())
        emit dialogTranspositionGroup(ui->spinWrite->value(), ui->spinNmin->value(), ui->spinNmax->value(), TranspositionGroupOptions::Write);
    /*if (ui->btnCycle->isChecked())
        emit dialogTranspositionGroup(ui->spinCycle->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt(), MebiusFunctionOptions::Cycle);
    if (ui->btnInverse->isChecked())
        emit dialogTranspositionGroup(ui->spinInverse->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt(), MebiusFunctionOptions::Inverse);
    if (ui->btnMod2->isChecked())
        emit dialogTranspositionGroup(ui->spinMod2->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt(), MebiusFunctionOptions::Mod2);
    if (ui->btnMultiply->isChecked())
        emit dialogTranspositionGroup(ui->spinMultiply->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt(), MebiusFunctionOptions::Multiply);
    */
this->close();
}

void DialogTranspositionGroup::on_buttonBox_rejected()
{
    close();
}

void DialogTranspositionGroup::on_btnWrite_clicked()
{
    if (ui->btnWrite->isChecked()) {
        ui->spinWrite->setEnabled(true);
    } else {
        ui->spinWrite->setDisabled(true);
    }
}

void DialogTranspositionGroup::on_btnMultiply_clicked()
{

}

void DialogTranspositionGroup::on_btnInverse_clicked()
{

}

void DialogTranspositionGroup::on_btnCycle_clicked()
{

}

void DialogTranspositionGroup::on_btnMod2_clicked()
{

}

void DialogTranspositionGroup::on_btnCount_clicked()
{

}

void DialogTranspositionGroup::on_btnOrder_clicked()
{

}
