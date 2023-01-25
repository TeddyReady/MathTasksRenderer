#include "dialogset.h"
#include "ui_dialogset.h"

const int DialogSet::numOfSets = 3, DialogSet::numOfOperators = 4;

DialogSet::DialogSet(QWidget *parent, bool mode) :
    QDialog(parent),
    ui(new Ui::DialogSet)
{
    uploadUI();
    uploadData();

    if (mode) {
        ui->buttonBox->button(QDialogButtonBox::Cancel)->deleteLater();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Сгенерировать задания");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon());
        isCancelExist = false;
    } else isCancelExist = true;
}

DialogSet::~DialogSet()
{
    delete ui;
}

void DialogSet::on_buttonBox_accepted()
{
    /*QRandomGenerator *gen = QRandomGenerator::global();
    for (int i = 0; i < ui->spinBox->value();) {
        int posS = gen->bounded(0, numOfSets), posO = gen->bounded(1, numOfOperators + 1);
        QTextStream in(&dataBase);
        operation = in.readLine().split(",").at(posO);
        for (int i = 0; i < posS; i++) {
            in.readLine();
        }
        QStringList tmp = in.readLine().split(",");
        set = static_cast<Set>(QString(tmp.at(0)).toInt());
        type = static_cast<SetType>(QString(tmp.at(posO)).toInt());
        dataBase.close();
        bool isFind = true;
        for (int i = 0; i < data.size(); i++) {
            if (getCode(set) == std::get<0>(data[i]) && operation == std::get<1>(data[i])) {
                isFind = false;
                break;
            }
        }
        if (isFind) {
            data.push_back(std::make_tuple(getCode(set), operation, getCode(type)));
            i++;
        }
    }
    emit dialogSetMeta(ui->spinBox->value());
    emit dialogSet(ui->spinBox->value(), data);
    if (isCancelExist) close();*/
}

void DialogSet::on_buttonBox_rejected()
{
    close();
}

void DialogSet::uploadUI()
{
    ui->setupUi(this);
    ui->btnCheck->setChecked(false);
    ui->spinCheck->setDisabled(true);
    ui->btnOper->setChecked(false);
    ui->spinOper->setDisabled(true);
    ui->btnAbel->setChecked(false);
    ui->spinAbel->setDisabled(true);
    ui->btnAssociate->setChecked(false);
    ui->spinAssociate->setDisabled(true);
    ui->btnNeutral->setChecked(false);
    ui->spinNeutral->setDisabled(true);
}

void DialogSet::uploadData()
{
    QFile dataBase(":/data/set.csv");
    if (!dataBase.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, "Внимание!", "Файл не существует!");
        return;
    } else {
        QTextStream in(&dataBase);
        //Распределяем операторы
        std::vector<QString> operators;
        QStringList operLine = in.readLine().split(",");
        operLine.removeAt(0);
        for (auto member: operLine) {
            operators.emplace_back(member);
        }
        for (int i = 0; i < numOfSets; i++) {
            QStringList tmp = in.readLine().split(",");
            for (int j = 0; j < operators.size(); j++)
                baseData.emplace_back(std::make_tuple(tmp.at(0), operators[j], tmp.at(j + 1)));

        } dataBase.close();
    }
}

QString DialogSet::getCode(Set set) const
{
    switch (set) {
    case Set::N:
        return "\\mathbb{N}";

    case Set::No:
        return "\\mathbb{N}_0";

    case Set::Z:
        return "\\mathbb{Z}";
    }
}
QString DialogSet::getCode(SetType type) const
{
    switch (type) {
    case SetType::Set:
        return "Множество";

    case SetType::Groupoid:
        return "Группоид";

    case SetType::Semigroup:
        return "Полугруппа";

    case SetType::Monoid:
        return "Моноид";

    case SetType::Group:
        return "Группа";
    } return "";
}

void DialogSet::on_btnCheck_clicked()
{
    if (ui->btnCheck->isChecked()) {
        ui->btnCheck->setChecked(true);
        ui->spinCheck->setEnabled(true);
    } else {
        ui->btnCheck->setChecked(false);
        ui->spinCheck->setDisabled(true);
    }
}


void DialogSet::on_btnOper_clicked()
{
    if (ui->btnOper->isChecked()) {
        ui->btnOper->setChecked(true);
        ui->spinOper->setEnabled(true);
    } else {
        ui->btnOper->setChecked(false);
        ui->spinOper->setDisabled(true);
    }
}


void DialogSet::on_btnAbel_clicked()
{
    if (ui->btnAbel->isChecked()) {
        ui->btnAbel->setChecked(true);
        ui->spinAbel->setEnabled(true);
    } else {
        ui->btnAbel->setChecked(false);
        ui->spinAbel->setDisabled(true);
    }
}


void DialogSet::on_btnAssociate_clicked()
{
    if (ui->btnAssociate->isChecked()) {
        ui->btnAssociate->setChecked(true);
        ui->spinAssociate->setEnabled(true);
    } else {
        ui->btnAssociate->setChecked(false);
        ui->spinAssociate->setDisabled(true);
    }
}


void DialogSet::on_btnNeutral_clicked()
{
    if (ui->btnNeutral->isChecked()) {
        ui->btnNeutral->setChecked(true);
        ui->spinNeutral->setEnabled(true);
    } else {
        ui->btnNeutral->setChecked(false);
        ui->spinNeutral->setDisabled(true);
    }
}

