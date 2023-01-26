#include "dialogset.h"
#include "ui_dialogset.h"

const int DialogSet::numOfSets = 6, DialogSet::numOfOperators = 4;

DialogSet::DialogSet(QWidget *parent, bool mode) :
    QDialog(parent), count(0), gen(QRandomGenerator::global()),
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
    delete gen;
    delete ui;
}

void DialogSet::on_buttonBox_accepted()
{
    if (ui->btnCheck->isChecked()) count += ui->spinCheck->value();
    if (ui->btnOper->isChecked()) count += ui->spinOper->value();
    if (ui->btnAbel->isChecked()) count += ui->spinAbel->value();
    if (ui->btnAssociate->isChecked()) count += ui->spinAssociate->value();
    if (ui->btnNeutral->isChecked()) count += ui->spinNeutral->value();
    emit dialogSetMeta(count); count = 0;

    if (ui->btnCheck->isChecked() && ui->spinCheck->value()) {
        set_type data;
        for(int i = 0; i < ui->spinCheck->value();) {
            size_t index = static_cast<size_t>(gen->bounded(0, baseData.size()));
            int answer = QString(std::get<2>(baseData[index])).toInt() / 1000;
            if (answer) {

                QString curSet = std::get<0>(baseData[index]);
                QString curOper = std::get<1>(baseData[index]);

                data.emplace_back(std::make_tuple(getCode(static_cast<Set>(curSet.toInt())),
                                                  curOper,
                                                  getCode(static_cast<SetType>(answer))));
                ++i;
            }
        }
        emit dialogSet(ui->spinCheck->value(), data, SetOptions::Check);
    }
    if (ui->btnOper->isChecked() && ui->spinOper->value())
        generateTasks(ui->spinOper->value(), SetOptions::Oper);

    if (ui->btnAbel->isChecked() && ui->spinAbel->value())
        generateTasks(ui->spinAbel->value(), SetOptions::Abel);

    if (ui->btnAssociate->isChecked() && ui->spinAssociate->value())
        generateTasks(ui->spinAssociate->value(), SetOptions::Associate);

    if (ui->btnNeutral->isChecked() && ui->spinNeutral->value())
        generateTasks(ui->spinNeutral->value(), SetOptions::Neutral);


    if (isCancelExist) close();
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

void DialogSet::generateTasks(const int &countOfTasks, const SetOptions &option)
{
    set_type data;
    for(int i = 0; i < countOfTasks; ++i) {
        size_t index = static_cast<size_t>(gen->bounded(0, baseData.size()));
        QString curSet = std::get<0>(baseData[index]);
        QString curOper = std::get<1>(baseData[index]);
        data.emplace_back(std::make_tuple(getCode(static_cast<Set>(curSet.toInt())),
                                curOper, std::get<2>(baseData[index])));
    }
    emit dialogSet(countOfTasks, data, option);
}

QString DialogSet::getCode(const Set &set) const
{
    switch (set) {
    case Set::N:
        return "\\mathbb{N}";

    case Set::No:
        return "\\mathbb{N}_0";

    case Set::Z:
        return "\\mathbb{Z}";

    case Set::Z0:
        return "\\mathbb{Z} \\backslash \\{ 0 \\}";

    case Set::Q:
        return "\\mathbb{Q}";

    case Set::Q0:
        return "\\mathbb{Q} \\backslash \\{ 0 \\}";
    }
}
QString DialogSet::getCode(const SetType &type) const
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

