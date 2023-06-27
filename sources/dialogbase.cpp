#include "dialogbase.h"
#include "ui_dialogbase.h"
DialogBase::DialogBase(AllTasks curTask, bool deleteMode, QWidget *parent) :
    task(curTask), deleteMode(deleteMode), QDialog(parent), ui(new Ui::DialogBase)
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

BaseWidget::BaseWidget(const QString &cbName, ExoticWidget type, QWidget *parent) :
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

void BaseWidget::setExoticOptions(const ExoticWidget &type)
{
    switch (type) {
    case ExoticWidget::Transposition:
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
    case ExoticWidget::None:
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
        if (optionName == "Размер") {
            sbMin->setMinimum(2);
            sbMax->setMaximum(8);
            sbMin->setValue(2);
            sbMax->setValue(4);
        } else {
            sbMin->setMinimum(-999);
            sbMax->setMaximum(999);
            sbMin->setValue(-10);
            sbMax->setValue(10);
        } return;
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
        addItem(Base, "Записать подстановку указанным способом", ExoticWidget::Transposition);
        addItem(Base, "Произведение подстановок", ExoticWidget::Transposition);
        addItem(Base, "Найти обратную", ExoticWidget::Transposition);
        addItem(Base, "Вычислить цикловой тип", ExoticWidget::Transposition);
        addItem(Base, "Вычислить четность", ExoticWidget::Transposition);
        addItem(Base, "Вычислить количество беспорядков", ExoticWidget::Transposition);
        addItem(Base, "Вычислить порядок подстановки", ExoticWidget::Transposition);
        addItem(Base, "Разложить перестановку в произведение транспозиций", ExoticWidget::Transposition);
        addItem(Base, "Разложить в произведение транспозиций соседних элементов", ExoticWidget::Transposition);
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
        addItem(Gen, "Размер");
        addItem(Gen, "Коэффициенты");
        addItem(Base, "Сумма матриц");
        addItem(Base, "Разность матриц");
        addItem(Base, "Умножение матриц");
        addItem(Base, "Найти обратную к матрице");
        addItem(Base, "Вычислить детерминант матрицы");
        break;
    case AllTasks::RingResidue:
        addItem(Gen);
        addItem(Base, "Количество образующих");
        addItem(Base, "Возведение числа в степень по модулю");
        addItem(Base, "Вычисление порядка элемента");
        addItem(Base, "Линейное сравнение");
        addItem(Base, "Квадратичное сравнение по простому модулю");
        addItem(Base, "Квадратичное сравнение по составному модулю");
        break;
    case AllTasks::RingOfMembers:
        addItem(Gen, "Степень");
        addItem(Gen, "Мощность кольца");
        addItem(Base, "Сложение многочленов");
        addItem(Base, "Умножение многочленов");
        addItem(Base, "Деление многочленов с остатком");
        addItem(Base, "Вычисление НОД многочленов");
        break;
    }
    if (ui->genWidgetLayout->isEmpty()) ui->lblGen->hide();
}

void DialogBase::addItem(WidgetRole role, const QString &name, ExoticWidget type)
{
    switch (role) {
    case Gen:
        ui->genWidgetLayout->addWidget(new GenWidget(task, name));
        break;
    case Base:
        BaseWidget *pWidget = new BaseWidget(name, type, this);
        ui->baseWidgetLayout->addWidget(pWidget->getCheckBox());
        if (type != ExoticWidget::None) ui->baseWidgetLayout->addWidget(pWidget->getPushButton());
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
    case AllTasks::Matrix:
    case AllTasks::RingOfMembers:
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
