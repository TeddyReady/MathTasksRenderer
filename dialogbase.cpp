#include "dialogbase.h"
#include "ui_dialogbase.h"
DialogBase::DialogBase(AllTasks curTask, bool deleteMode, QWidget *parent) :
    task(curTask), QDialog(parent), ui(new Ui::DialogBase)
{
    ui->setupUi(this);
    setWindowTitle("Выберите настройки генерации");
    uploadUI();

    if (!deleteMode) {
        ui->buttonBox->button(QDialogButtonBox::Cancel)->hide();
        ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Сгенерировать задания");
        ui->buttonBox->button(QDialogButtonBox::Ok)->setIcon(QIcon());
    }

    connect(ui->buttonBox, &QDialogButtonBox::accepted, [&, deleteMode](){
        //Sending Meta Info
        int countOfTasks = 0;
        for (std::size_t i = 0; i < widgets.size(); ++i)
        {
            if (widgets[i]->isChecked()) countOfTasks += widgets[i]->getCount();
        } if (!countOfTasks) deleteLater();
        emit sendingMetaInfo(countOfTasks, isRepeatable(), getTaskText());

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
                if (static_cast<AllTasks>(task) == AllTasks::SymbolLegandre ||
                    static_cast<AllTasks>(task) == AllTasks::SymbolJacobi) {
                    data.emplace_back(ranges[0].first);
                    data.emplace_back(ranges[0].second);
                    data.emplace_back(ranges[1].first);
                    data.emplace_back(ranges[1].second);
                } else if (static_cast<AllTasks>(task) != AllTasks::Set) {
                    data.emplace_back(ranges[0].first);
                    data.emplace_back(ranges[0].second);
                }
                emit sendingData(data, task, i, widgets[i]->getViewMode());
            }
        }
        if (deleteMode) deleteLater();
    });
    connect(ui->buttonBox, &QDialogButtonBox::rejected, [this](){ deleteLater(); });

    show();
}

DialogBase::~DialogBase()
{
    delete ui;
}

BaseWidget::BaseWidget(const QString &cbName, bool isTP, QWidget *parent) :
    QWidget(parent), vm(ViewMode::None), isTP(isTP)
{
    setLayout(new QGridLayout(this));
    cb = new QCheckBox(cbName, this);
    sb = new QSpinBox(this);
    sb->setDisabled(true);
    layout()->addWidget(cb);
    if (isTP) {
        pb = new QPushButton("Выбрать вид...", this);
        pb->setMenu(new QMenu(pb));
        pb->menu()->addAction(new QAction("Канонический вид", pb->menu()));
        pb->menu()->addAction(new QAction("Циклический вид", pb->menu()));
        layout()->addWidget(pb);
        connect(cb, &QCheckBox::clicked, [&](){
            if (cb->isChecked()) pb->setEnabled(true);
            else pb->setDisabled(true);
        });
        connect(pb->menu()->actions().at(0), &QAction::triggered, [&](){
            pb->setText("Канонический вид");
            vm = ViewMode::Standart;
            sb->setEnabled(true);
        });
        connect(pb->menu()->actions().at(1), &QAction::triggered, [&](){
            pb->setText("Циклический вид");
            vm = ViewMode::Cycle;
            sb->setEnabled(true);
        });
    } else {
        connect(cb, &QCheckBox::clicked, [&](){
            if (cb->isChecked()) sb->setEnabled(true);
            else sb->setDisabled(true);
        });
    }
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
        sbMin->setValue(1);
        sbMin->setMinimum(1);
        sbMax->setValue(100);
        sbMax->setMaximum(1000000);
        return;

    case AllTasks::SymbolLegandre:
    case AllTasks::SymbolJacobi:
        if (optionName == "a") {
            sbMin->setValue(-100);
            sbMin->setMinimum(-100000);
            sbMax->setValue(100);
            sbMax->setMaximum(100000);
        } else {
            sbMin->setValue(2);
            sbMin->setMinimum(1);
            sbMax->setValue(100);
            sbMax->setMaximum(100000);
        }
        return;

    case AllTasks::TranspositionGroup:
        sbMin->setValue(3);
        sbMin->setMinimum(2);
        sbMax->setValue(10);
        sbMax->setMaximum(30);
        return;

    default:
        return;
    }
}

void DialogBase::uploadUI()
{
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
        addItem(Base, "Записать подстановку указанным способом", true);
        addItem(Base, "Произведение подстановок", true);
        addItem(Base, "Найти обратную", true);
        addItem(Base, "Вычислить цикловой тип", true);
        addItem(Base, "Вычислить четность", true);
        addItem(Base, "Вычислить количество беспорядков", true);
        addItem(Base, "Вычислить порядок подстановки", true);
        addItem(Base, "Разложить перестановку в произведение транспозиций", true);
        addItem(Base, "Разложить в произведение транспозиций соседних элементов", true);
        break;
    case AllTasks::Set:
        addItem(Base, "Описание алгебраической структуры");
        addItem(Base, "Проверка операции на множестве");
        addItem(Base, "Коммутативность операции");
        addItem(Base, "Ассоциативность операции");
        addItem(Base, "Существование нейтрального");
        break;
    }

    if (ui->genWidgetLayout->isEmpty()) ui->lblGen->hide();
}

bool DialogBase::isRepeatable() const
{
    switch (task) {
    case AllTasks::TranspositionGroup:
    case AllTasks::Set:
        return false;

    default:
        return true;
    }
}

QString DialogBase::getTaskText() const
{
    switch (task) {
    case AllTasks::EulerFunction:
        return QString("Вычислите функцию Эйлера:");
    case AllTasks::MebiusFunction:
        return QString("Вычислите функцию Мёбиуса:");
    case AllTasks::SymbolLegandre:
        return QString("Вычислите символ Лежандра:");
    case AllTasks::SymbolJacobi:
        return QString("Вычислите символ Якоби:");

    default:
        return QString("");
    }
}

void DialogBase::addItem(WidgetRole role, const QString &name, bool option)
{
    switch (role) {
    case Gen:
        ui->genWidgetLayout->addWidget(new GenWidget(task, name));
        break;
    case Base:
        BaseWidget *pWidget = new BaseWidget(name, option, this);
        ui->baseWidgetLayout->addWidget(pWidget->getCheckBox());
        if (option) ui->baseWidgetLayout->addWidget(pWidget->getPushButton());
        ui->baseWidgetLayout->addWidget(pWidget->getSpinBox());
        widgets.emplace_back(pWidget);
        break;
    }
}
