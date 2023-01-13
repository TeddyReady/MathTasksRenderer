#include "dialogset.h"
#include "ui_dialogset.h"
#define OPERNUM 4
#define SETNUM 3

DialogSet::DialogSet(QWidget *parent, bool mode) :
    QDialog(parent),
    ui(new Ui::DialogSet)
{
    ui->setupUi(this);
    ui->spinBox->setMaximum(12);
    if (mode) {
        ui->buttonBox->button(QDialogButtonBox::Cancel)->deleteLater();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Сгенерировать задания");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon());
        isCancelExist = false;
    } else isCancelExist = true;
    data = new set_type;
}

DialogSet::~DialogSet()
{
    data = nullptr;
    delete ui;
}

void DialogSet::on_buttonBox_accepted()
{
    QRandomGenerator *gen = QRandomGenerator::global();
    for (int i = 0; i < ui->spinBox->value();) {
        QString operation; Set set; SetType type;
        QFile dataBase(":/data/set.csv");
        if (!dataBase.open(QFile::ReadOnly)) {
            QMessageBox::warning(this, "Внимание!", "Файл не существует!");
            break;
        } else {
            int posS = gen->bounded(0, SETNUM), posO = gen->bounded(1, OPERNUM + 1);
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
            for (int i = 0; i < data->size(); i++) {
                if (getCode(set) == std::get<0>((*data)[i]) && operation == std::get<1>((*data)[i])) {
                    isFind = false;
                    break;
                }
            }
            if (isFind) {
                data->push_back(std::make_tuple(getCode(set), operation, getCode(type)));
                i++;
            }
        }
    }
    emit dialogSetMeta(ui->spinBox->value());
    emit dialogSet(ui->spinBox->value(), data);
    if (isCancelExist) close();
}

void DialogSet::on_buttonBox_rejected()
{
    close();
}

QString DialogSet::getCode(Set set)
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
QString DialogSet::getCode(SetType type)
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
