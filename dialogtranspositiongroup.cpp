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
    ui->typeWrite->setDisabled(true);
    ui->btnMultiply->setChecked(false);
    ui->spinMultiply->setDisabled(true);
    ui->typeMultiply->setDisabled(true);
    ui->btnInverse->setChecked(false);
    ui->spinInverse->setDisabled(true);
    ui->typeInverse->setDisabled(true);
    ui->btnCycle->setChecked(false);
    ui->spinCycle->setDisabled(true);
    ui->typeCycle->setDisabled(true);
    ui->btnMod2->setChecked(false);
    ui->spinMod2->setDisabled(true);
    ui->typeMod2->setDisabled(true);
    ui->btnOrder->setChecked(false);
    ui->spinOrder->setDisabled(true);
    ui->typeOrder->setDisabled(true);
    ui->btnCount->setChecked(false);
    ui->spinCount->setDisabled(true);
    ui->typeCount->setDisabled(true);

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
        emit dialogTranspositionGroup(ui->spinWrite->value(), ui->spinNmin->value(), ui->spinNmax->value() + 1, TranspositionGroupOptions::Write, taskWrite);
    /*if (ui->btnCycle->isChecked())
        emit dialogTranspositionGroup(ui->spinCycle->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, MebiusFunctionOptions::Cycle);
    if (ui->btnInverse->isChecked())
        emit dialogTranspositionGroup(ui->spinInverse->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, MebiusFunctionOptions::Inverse);
    if (ui->btnMod2->isChecked())
        emit dialogTranspositionGroup(ui->spinMod2->value(), ui->lineMin->text().toInt(), ui->lineMax->text().toInt() + 1, MebiusFunctionOptions::Mod2);
    */
    if (ui->btnMultiply->isChecked())
        emit dialogTranspositionGroup(ui->spinMultiply->value(), ui->spinNmin->value(), ui->spinNmax->value() + 1, TranspositionGroupOptions::Multiply, taskMultiply);
    this->close();
}

void DialogTranspositionGroup::on_buttonBox_rejected()
{
    close();
}

void DialogTranspositionGroup::on_btnWrite_clicked()
{
    if (ui->btnWrite->isChecked()) {
        ui->typeWrite->setEnabled(true);

        QMenu *subMenu = new QMenu(ui->typeWrite);
        QAction *viewDefault = new QAction("Канонический вид");
        QAction *viewCycle = new QAction("Циклический вид");
        viewDefault->setCheckable(true);
        viewCycle->setCheckable(true);
        subMenu->addAction(viewDefault);
        subMenu->addAction(viewCycle);
        ui->typeWrite->setMenu(subMenu);
        connect(viewDefault, &QAction::triggered, [=](){
            ui->spinWrite->setEnabled(true);
            if (viewDefault->isChecked()) {
                viewCycle->setChecked(false);
            }
            viewDefault->setChecked(true);
            ui->typeWrite->setText("Канонический вид");
            taskWrite = ViewMode::Standart;
        });
        connect(viewCycle, &QAction::triggered, [=](){
            ui->spinWrite->setEnabled(true);
            if (viewCycle->isChecked()) {
                viewDefault->setChecked(false);
            }
            viewCycle->setChecked(true);
            ui->typeWrite->setText("Циклический вид");
            taskWrite = ViewMode::Cycle;
        });

    } else {
        ui->spinWrite->setDisabled(true);
        ui->typeWrite->setDisabled(true);
    }
}

void DialogTranspositionGroup::on_btnMultiply_clicked()
{
    if (ui->btnMultiply->isChecked()) {
        ui->typeMultiply->setEnabled(true);

        QMenu *subMenu = new QMenu(ui->typeMultiply);
        QAction *viewDefault = new QAction("Канонический вид");
        QAction *viewCycle = new QAction("Циклический вид");
        viewDefault->setCheckable(true);
        viewCycle->setCheckable(true);
        subMenu->addAction(viewDefault);
        subMenu->addAction(viewCycle);
        ui->typeMultiply->setMenu(subMenu);
        connect(viewDefault, &QAction::triggered, [=](){
            ui->spinMultiply->setEnabled(true);
            if (viewDefault->isChecked()) {
                viewCycle->setChecked(false);
            }
            viewDefault->setChecked(true);
            ui->typeMultiply->setText("Канонический вид");
            taskMultiply = ViewMode::Standart;
        });
        connect(viewCycle, &QAction::triggered, [=](){
            ui->spinMultiply->setEnabled(true);
            if (viewCycle->isChecked()) {
                viewDefault->setChecked(false);
            }
            viewCycle->setChecked(true);
            ui->typeMultiply->setText("Циклический вид");
            taskMultiply = ViewMode::Cycle;
        });

    } else {
        ui->spinMultiply->setDisabled(true);
        ui->typeMultiply->setDisabled(true);
    }
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
