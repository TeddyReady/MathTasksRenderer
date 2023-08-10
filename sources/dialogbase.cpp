#include "dialogbase.h"
#include "ui_dialogbase.h"
DialogBase::DialogBase(AllTasks curTask, bool deleteMode, QWidget *parent) : QDialog(parent),
    task(curTask), deleteMode(deleteMode), isNeedEmptyColumn(false), ui(new Ui::DialogBase)
{
    ui->setupUi(this);
    setWindowTitle("Выберите настройки генерации");
    uploadUI();

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &DialogBase::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &DialogBase::reject);

    show();
}

DialogBase::~DialogBase()
{
    delete ui;
}

BaseWidget::BaseWidget(const QString &cbName, ExoticOption type, QWidget *parent) :
    QWidget(parent), exoticOption(0), type(type)
{
    setLayout(new QGridLayout(this));
    cb = new QCheckBox(cbName, this);
    sb = new QSpinBox(this);
    sb->setDisabled(true);
    layout()->addWidget(cb);
    setExoticOptions(type);
    layout()->addWidget(sb);
}

GenWidget::GenWidget(AllTasks task, const QString &optionName, QWidget *parent) : QWidget(parent)
{
    sbMin = new QSpinBox(this);
    sbMax = new QSpinBox(this);
    loadSettings(task, optionName);

    setLayout(new QHBoxLayout(this));
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->addWidget(new QLabel("От", this));
    layout()->addWidget(sbMin);
    layout()->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    layout()->addWidget(new QLabel("До", this));
    layout()->addWidget(sbMax);
    if (optionName != "")
        layout()->addWidget(new QLabel(optionName, this));

    connect(sbMin, &QSpinBox::editingFinished, [&](){ sbMax->setMinimum(sbMin->value()); });
    connect(sbMax, &QSpinBox::editingFinished, [&](){ sbMin->setMaximum(sbMax->value()); });
}



void GenWidget::loadSettings(AllTasks task, const QString &optionName)
{
    switch (task) {
    case AllTasks::EulerFunction:
    case AllTasks::MebiusFunction:
        sbMin->setRange(1, 100);
        sbMax->setRange(1, 9999);
        sbMin->setValue(1);
        sbMax->setValue(100);
        return;

    case AllTasks::SymbolLegandre:
    case AllTasks::SymbolJacobi:
        if (optionName == "a") {
            sbMin->setRange(-9999, 100);
            sbMax->setRange(-100, 9999);
            sbMin->setValue(-100);
            sbMax->setValue(100);
        } else {
            sbMin->setMinimum(1);
            sbMax->setMaximum(100000);
            sbMin->setValue(2);
            sbMax->setValue(100);
        }
        return;

    case AllTasks::TranspositionGroup:
        sbMin->setMinimum(2);
        sbMax->setMaximum(30);
        sbMin->setValue(3);
        sbMax->setValue(10);
        return;
    case AllTasks::GroupProperties:
        sbMin->setMinimum(3);
        sbMax->setMaximum(20);
        sbMin->setValue(4);
        sbMax->setValue(12);
        return;
    case AllTasks::Matrix:
        sbMin->setMinimum(2);
        sbMax->setMaximum(8);
        sbMin->setValue(2);
        sbMax->setValue(4);
        return;
    case AllTasks::RingResidue:
        sbMin->setMinimum(3);
        sbMax->setMaximum(99);
        sbMin->setValue(5);
        sbMax->setValue(50);
        return;
    case AllTasks::RingOfMembers:
        if (optionName == "Степень") {
            sbMin->setMinimum(2);
            sbMax->setMaximum(8);
            sbMin->setValue(2);
            sbMax->setValue(6);
        } else {
            sbMin->setMinimum(2);
            sbMax->setMaximum(50);
            sbMin->setValue(2);
            sbMax->setValue(20);
        } return;
    default:
        return;
    }
}

void DialogBase::uploadUI()
{
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Сгенерировать задания");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon());
    if (!deleteMode)
        ui->buttonBox->button(QDialogButtonBox::Cancel)->deleteLater();
    else {
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Выход");
        ui->buttonBox->button(QDialogButtonBox::Cancel)->setIcon(QIcon());
    }

    switch (task) {
    case AllTasks::EulerFunction:
        addItem(Gen);
        addItem(Base, "По умолчанию");
        addItem(Base, "Простое");
        addItem(Base, "Степень простого");
        addItem(Base, "Произведение взаимно простых");
        addItem(Base, "Произведение степеней простых");
        break;

    case AllTasks::MebiusFunction:
        addItem(Gen);
        addItem(Base, "По умолчанию");
        addItem(Base, "Произведение взаимно простых");
        addItem(Base, "Произведение чётного числа различных простых");
        addItem(Base, "Произведение нечётного числа различных простых");
        addItem(Base, "Встречается степень простого числа");
        break;

    case AllTasks::SymbolLegandre:
    case AllTasks::SymbolJacobi:
        addItem(Gen, "a");
        addItem(Gen, "p");
        addItem(Base, "По умолчанию");
        addItem(Base, "Раскладывается в произведение простых");
        addItem(Base, "В качестве а = -1");
        addItem(Base, "В качестве а = 2");
        addItem(Base, "a и p взаимно просты и нечетны");
        break;

    case AllTasks::TranspositionGroup:
        ui->baseWidgetLayout->addWidget(new QLabel("Количество"), 0, 2);
        dynamic_cast<QLabel *>(ui->baseWidgetLayout->itemAt(1)->widget())->setText("Вид перестановки");
        addItem(Gen);
        addItem(Base, "Записать подстановку указанным способом", ExoticOption::Transposition);
        addItem(Base, "Произведение подстановок", ExoticOption::Transposition);
        addItem(Base, "Найти обратную", ExoticOption::Transposition);
        addItem(Base, "Вычислить цикловой тип", ExoticOption::Transposition);
        addItem(Base, "Вычислить четность", ExoticOption::Transposition);
        addItem(Base, "Вычислить количество беспорядков", ExoticOption::Transposition);
        addItem(Base, "Вычислить порядок подстановки", ExoticOption::Transposition);
        addItem(Base, "Разложить перестановку в произведение транспозиций", ExoticOption::Transposition);
        addItem(Base, "Разложить в произведение транспозиций соседних элементов", ExoticOption::Transposition);
        break;

    case AllTasks::Set:
        addItem(Base, "Описание алгебраической структуры");
        addItem(Base, "Проверка операции на множестве");
        addItem(Base, "Коммутативность операции");
        addItem(Base, "Ассоциативность операции");
        addItem(Base, "Существование нейтрального");
        break;

    case AllTasks::GroupProperties:
        addItem(Gen, "n");
        addItem(Base, "Перечислить подгруппы");
        addItem(Base, "Перечислить нормальные подгруппы");
        addItem(Base, "Нахождение центра группы");
        addItem(Base, "Вычислить порядок группы");
        addItem(Base, "Вычислить экспоненту группы");
        break;

    case AllTasks::Matrix:
        ui->baseWidgetLayout->addWidget(new QLabel("Количество"), 0, 2);
        dynamic_cast<QLabel *>(ui->baseWidgetLayout->itemAt(1)->widget())->setText("Алгебраическая структура");
        addItem(Gen, "Размер");
        addItem(Base, "Сумма матриц"        , ExoticOption::MatrixDefault);
        addItem(Base, "Разность матриц"     , ExoticOption::MatrixDefault);
        addItem(Base, "Умножение матриц"    , ExoticOption::MatrixDefault);
        addItem(Base, "Обратная матрица"    , ExoticOption::MatrixInverse);
        addItem(Base, "Детерминант матрицы" , ExoticOption::MatrixDefault);
        break;

    case AllTasks::RingResidue:
        isNeedEmptyColumn = true;
        ui->baseWidgetLayout->addWidget(new QLabel("Количество"), 0, 2);
        dynamic_cast<QLabel *>(ui->baseWidgetLayout->itemAt(1)->widget())->setText("Алгебраическая структура");
        addItem(Gen);
        addItem(Base, "Количество образующих", ExoticOption::ResidueDefault);
        addItem(Base, "Возведение числа в степень по модулю", ExoticOption::ResidueAll);
        addItem(Base, "Вычисление порядка элемента", ExoticOption::ResidueAll);
        addItem(Base, "Число решений линейного сравнения");
        addItem(Base, "Линейное сравнение");
        addItem(Base, "Число решений квадратичного сравнения по простому модулю");
        addItem(Base, "Квадратичное сравнение по простому модулю");
        addItem(Base, "Число решений квадратичного сравнения по составному модулю");
        addItem(Base, "Квадратичное сравнение по составному модулю");
        isNeedEmptyColumn = false;
        break;

    case AllTasks::RingOfMembers:
        ui->baseWidgetLayout->addWidget(new QLabel("Количество"), 0, 2);
        dynamic_cast<QLabel *>(ui->baseWidgetLayout->itemAt(1)->widget())->setText("Алгебраическая структура");
        addItem(Gen, "Степень");
        addItem(Base, "Сложение многочленов"    , ExoticOption::MembersAll);
        addItem(Base, "Вычитание многочленов"   , ExoticOption::MembersAll);
        addItem(Base, "Умножение многочленов"   , ExoticOption::MembersAll);
        addItem(Base, "Целая часть от деления"  , ExoticOption::MembersFields);
        addItem(Base, "Деление с остатком"      , ExoticOption::MembersFields);
        addItem(Base, "НОД многочленов"         , ExoticOption::MembersFields);
        break;
    }
    if (ui->genWidgetLayout->isEmpty()) ui->lblGen->hide();
}

void DialogBase::addItem(WidgetRole role, const QString &name, ExoticOption type)
{
    switch (role) {
    case Gen:
        ui->genWidgetLayout->addWidget(new GenWidget(task, name));
        break;
    case Base:
        BaseWidget *pWidget = new BaseWidget(name, type, this);
        ui->baseWidgetLayout->addWidget(pWidget->getCheckBox());
        if (type != ExoticOption::None) ui->baseWidgetLayout->addWidget(pWidget->getPushButton());
        else if (isNeedEmptyColumn) ui->baseWidgetLayout->addWidget(new QWidget(this));
        ui->baseWidgetLayout->addWidget(pWidget->getSpinBox());
        widgets.emplace_back(pWidget);
        break;
    }
}

bool DialogBase::isHaveMoreGens()
{
    switch (task) {
    case AllTasks::SymbolLegandre:
    case AllTasks::SymbolJacobi:
        return true;

    default:
        return false;
    }
}

void DialogBase::accept()
{
    //Sending Meta Info
    int countOfTasks = 0;
    for (std::size_t i = 0; i < widgets.size(); ++i)
    {
        if (widgets[i]->isChecked()) countOfTasks += widgets[i]->getCount();
    } if (!countOfTasks) deleteLater();
    emit sendingMetaInfo(countOfTasks);

    //Upload Ranges
    for (int i = 0; i < ui->genWidgetLayout->count(); ++i)
    {
        GenWidget *pWidget = dynamic_cast<GenWidget*>(ui->genWidgetLayout->itemAt(i)->widget());
        ranges.emplace_back(std::move(pWidget->getRange()));
    }

    //Sending OtherInfo
    for (std::size_t i = 0; i < widgets.size(); ++i)
    {
        std::vector<int> data;
        if (widgets[i]->isChecked()) {
            data.emplace_back(widgets[i]->getCount());
            if (isHaveMoreGens()) {
                data.emplace_back(ranges[0].first);
                data.emplace_back(ranges[0].second);
                data.emplace_back(ranges[1].first);
                data.emplace_back(ranges[1].second);
            } else if (static_cast<AllTasks>(task) != AllTasks::Set) {
                data.emplace_back(ranges[0].first);
                data.emplace_back(ranges[0].second);
            }
            emit sendingData(data, task, i, widgets[i]->getExoticOption());
        }
    }
    if (deleteMode) QDialog::accept();
}
void DialogBase::reject()
{
    QDialog::reject();
}

void BaseWidget::setExoticOptions(const ExoticOption &type)
{
    switch (type) {
    case ExoticOption::Transposition:
        exoticOption = static_cast<int>(ViewMode::None);
        pb = new QPushButton("Выбрать вид...", this);
        pb->setMenu(new QMenu(pb));
        pb->menu()->addAction(new QAction("Канонический вид", pb->menu()));
        pb->menu()->addAction(new QAction("Циклический вид", pb->menu()));
        pb->setDisabled(true);
        layout()->addWidget(pb);
        connect(cb, &QCheckBox::clicked, [&](){
            if (cb->isChecked()) pb->setEnabled(true);
            else pb->setDisabled(true);
            sb->setDisabled(true);
            sb->setValue(0);
        });
        connect(pb->menu()->actions().at(0), &QAction::triggered, [&](){
            pb->setText("Канонический вид");
            exoticOption = static_cast<int>(ViewMode::Standart);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(1), &QAction::triggered, [&](){
            pb->setText("Циклический вид");
            exoticOption = static_cast<int>(ViewMode::Cycle);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        break;
    case ExoticOption::MatrixDefault:
        exoticOption = static_cast<int>(Set::N);
        pb = new QPushButton("Выбрать поле чисел...", this);
        pb->setMenu(new QMenu(pb));
        pb->menu()->addAction(new QAction("Действительные", pb->menu()));
        pb->menu()->addAction(new QAction("Целые", pb->menu()));
        pb->menu()->addAction(new QAction("Кольцо вычетов", pb->menu()));
        pb->menu()->addAction(new QAction("Поле вычетов", pb->menu()));
        pb->menu()->addAction(new QAction("Комплексные", pb->menu()));
        pb->menu()->addAction(new QAction("Целые Гауссовы", pb->menu()));
        pb->setDisabled(true);
        layout()->addWidget(pb);
        connect(cb, &QCheckBox::clicked, [&](){
            if (cb->isChecked()) pb->setEnabled(true);
            else pb->setDisabled(true);
            sb->setDisabled(true);
            sb->setValue(0);
        });
        connect(pb->menu()->actions().at(0), &QAction::triggered, [&](){
            pb->setText("Действительные");
            exoticOption = static_cast<int>(Set::R);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(1), &QAction::triggered, [&](){
            pb->setText("Целые");
            exoticOption = static_cast<int>(Set::Z);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(2), &QAction::triggered, [&](){
            pb->setText("Кольцо вычетов");
            exoticOption = static_cast<int>(Set::Zn);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(3), &QAction::triggered, [&](){
            pb->setText("Поле вычетов");
            exoticOption = static_cast<int>(Set::Zp);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(4), &QAction::triggered, [&](){
            pb->setText("Комплексные");
            exoticOption = static_cast<int>(Set::C);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(5), &QAction::triggered, [&](){
            pb->setText("Целые Гауссовы");
            exoticOption = static_cast<int>(Set::Z_i);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        break;

    case ExoticOption::MatrixInverse:
        exoticOption = static_cast<int>(Set::N);
        pb = new QPushButton("Выбрать поле чисел...", this);
        pb->setMenu(new QMenu(pb));
        pb->menu()->addAction(new QAction("Действительные", pb->menu()));
        pb->menu()->addAction(new QAction("Поле вычетов", pb->menu()));
        pb->menu()->addAction(new QAction("Комплексные", pb->menu()));
        pb->setDisabled(true);
        layout()->addWidget(pb);
        connect(cb, &QCheckBox::clicked, [&](){
            if (cb->isChecked()) pb->setEnabled(true);
            else pb->setDisabled(true);
            sb->setDisabled(true);
            sb->setValue(0);
        });
        connect(pb->menu()->actions().at(0), &QAction::triggered, [&](){
            pb->setText("Действительные");
            exoticOption = static_cast<int>(Set::R);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(1), &QAction::triggered, [&](){
            pb->setText("Поле вычетов");
            exoticOption = static_cast<int>(Set::Zp);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(2), &QAction::triggered, [&](){
            pb->setText("Комплексные");
            exoticOption = static_cast<int>(Set::C);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        break;

    case ExoticOption::ResidueAll:
        exoticOption = static_cast<int>(ResidueType::Zn);
        pb = new QPushButton("Выбрать алгебраическую структуру...", this);
        pb->setMenu(new QMenu(pb));
        pb->menu()->addAction(new QAction("Кольцо вычетов", pb->menu()));
        pb->menu()->addAction(new QAction("Поле вычетов", pb->menu()));
        pb->menu()->addAction(new QAction("Мультипликативная группа вычетов", pb->menu()));
        pb->setDisabled(true);
        layout()->addWidget(pb);
        connect(cb, &QCheckBox::clicked, [&](){
            if (cb->isChecked()) pb->setEnabled(true);
            else pb->setDisabled(true);
            sb->setDisabled(true);
            sb->setValue(0);
        });
        connect(pb->menu()->actions().at(0), &QAction::triggered, [&](){
            pb->setText("Кольцо вычетов");
            exoticOption = static_cast<int>(ResidueType::Zn);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(1), &QAction::triggered, [&](){
            pb->setText("Поле вычетов");
            exoticOption = static_cast<int>(ResidueType::Zp);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(2), &QAction::triggered, [&](){
            pb->setText("Мультипликативная группа вычетов");
            exoticOption = static_cast<int>(ResidueType::MultiGroup_Zn);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        break;

    case ExoticOption::ResidueDefault:
        exoticOption = static_cast<int>(ResidueType::Zn);
        pb = new QPushButton("Выбрать алгебраическую структуру...", this);
        pb->setMenu(new QMenu(pb));
        pb->menu()->addAction(new QAction("Кольцо вычетов", pb->menu()));
        pb->menu()->addAction(new QAction("Поле вычетов", pb->menu()));
        pb->setDisabled(true);
        layout()->addWidget(pb);
        connect(cb, &QCheckBox::clicked, [&](){
            if (cb->isChecked()) pb->setEnabled(true);
            else pb->setDisabled(true);
            sb->setDisabled(true);
            sb->setValue(0);
        });
        connect(pb->menu()->actions().at(0), &QAction::triggered, [&](){
            pb->setText("Кольцо вычетов");
            exoticOption = static_cast<int>(ResidueType::Zn);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(1), &QAction::triggered, [&](){
            pb->setText("Поле вычетов");
            exoticOption = static_cast<int>(ResidueType::Zp);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        break;

    case ExoticOption::MembersAll:
        exoticOption = static_cast<int>(Set::Z);
        pb = new QPushButton("Выбрать поле чисел...", this);
        pb->setMenu(new QMenu(pb));
        pb->menu()->addAction(new QAction("Действительные", pb->menu()));
        pb->menu()->addAction(new QAction("Целые", pb->menu()));
        pb->menu()->addAction(new QAction("Кольцо вычетов", pb->menu()));
        pb->menu()->addAction(new QAction("Поле вычетов", pb->menu()));
        pb->menu()->addAction(new QAction("Комплексные", pb->menu()));
        pb->menu()->addAction(new QAction("Целые Гауссовы", pb->menu()));
        pb->setDisabled(true);
        layout()->addWidget(pb);
        connect(cb, &QCheckBox::clicked, [&](){
            if (cb->isChecked()) pb->setEnabled(true);
            else pb->setDisabled(true);
            sb->setDisabled(true);
            sb->setValue(0);
        });
        connect(pb->menu()->actions().at(0), &QAction::triggered, [&](){
            pb->setText("Действительные");
            exoticOption = static_cast<int>(Set::R);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(1), &QAction::triggered, [&](){
            pb->setText("Целые");
            exoticOption = static_cast<int>(Set::Z);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(2), &QAction::triggered, [&](){
            pb->setText("Кольцо вычетов");
            exoticOption = static_cast<int>(Set::Zn);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(3), &QAction::triggered, [&](){
            pb->setText("Поле вычетов");
            exoticOption = static_cast<int>(Set::Zp);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(4), &QAction::triggered, [&](){
            pb->setText("Комплексные");
            exoticOption = static_cast<int>(Set::C);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(5), &QAction::triggered, [&](){
            pb->setText("Целые Гауссовы");
            exoticOption = static_cast<int>(Set::Z_i);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        break;

    case ExoticOption::MembersFields:
        exoticOption = static_cast<int>(Set::R);
        pb = new QPushButton("Выбрать поле чисел...", this);
        pb->setMenu(new QMenu(pb));
        pb->menu()->addAction(new QAction("Действительные", pb->menu()));
        pb->menu()->addAction(new QAction("Поле вычетов", pb->menu()));
        pb->menu()->addAction(new QAction("Комплексные", pb->menu()));
        pb->setDisabled(true);
        layout()->addWidget(pb);
        connect(cb, &QCheckBox::clicked, [&](){
            if (cb->isChecked()) pb->setEnabled(true);
            else pb->setDisabled(true);
            sb->setDisabled(true);
            sb->setValue(0);
        });
        connect(pb->menu()->actions().at(0), &QAction::triggered, [&](){
            pb->setText("Действительные");
            exoticOption = static_cast<int>(Set::R);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(1), &QAction::triggered, [&](){
            pb->setText("Поле вычетов");
            exoticOption = static_cast<int>(Set::Zp);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        connect(pb->menu()->actions().at(2), &QAction::triggered, [&](){
            pb->setText("Комплексные");
            exoticOption = static_cast<int>(Set::C);
            sb->setEnabled(true);
            sb->setValue(1);
        });
        break;

    case ExoticOption::None:
        connect(cb, &QCheckBox::clicked, [&](){
            if (cb->isChecked()) {
                sb->setEnabled(true);
                sb->setValue(1);
            } else {
                sb->setDisabled(true);
                sb->setValue(0);
            }
        });
        break;
    }
}
