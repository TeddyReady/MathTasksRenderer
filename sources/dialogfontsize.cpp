#include "dialogfontsize.h"
#include "ui_dialogfontsize.h"

DialogFontSize::DialogFontSize(QString curTaskFont, QString curMathFont, QWidget *parent) :
    QDialog(parent), taskFont(curTaskFont), mathFont(curMathFont), ui(new Ui::DialogFontSize)
{
    ui->setupUi(this);
    mainPage = "\\color{sienna}{"
               "{TASK_FONT\\textbf{Сколько же будет...}}\\\\"
               "{MATH_FONT2 \\cdot 2 = ?}"
               "}";
    this->curMathFont = curMathFont;
    this->curTaskFont = curTaskFont;
    QString pageBuffer = mainPage;
    ui->cbType->setMaximumWidth(400);
    engine = new TeXEngine(ui->webPreview);
    engine->TeX2SVG(pageBuffer.replace("TASK_FONT", taskFont).replace("MATH_FONT", mathFont), true);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &DialogFontSize::accept);
    connect(ui->fontList, &QListWidget::itemClicked, [=](QListWidgetItem *item){
        QString tmp, fontName = item->text();
        if (fontName == "Крошечный") tmp = "\\tiny";
        else if (fontName == "Очень маленький") tmp = "\\Tiny";
        else if (fontName == "Маленький") tmp = "\\scriptsize";
        else if (fontName == "Мелкий") tmp = "\\small";
        else if (fontName == "Стандартный") tmp = "\\normalsize";
        else if (fontName == "Крупный") tmp = "\\large";
        else if (fontName == "Большой") tmp = "\\Large";
        else if (fontName == "Очень большой") tmp = "\\LARGE";
        else if (fontName == "Огромный") tmp = "\\huge";
        else tmp = "\\Huge";
        if (ui->cbType->currentText() == "Текст")
            taskFont = tmp;
        else mathFont = tmp;
        QString pageBuffer = mainPage;
        engine->TeX2SVG(pageBuffer.replace("TASK_FONT", taskFont).replace("MATH_FONT", mathFont), true);
    });
    connect(ui->cbType, &QComboBox::currentTextChanged, [&](){
        if (ui->cbType->currentText() == "Текст")
            selectFontSize(taskFont);
        else selectFontSize(mathFont);
    });

    selectFontSize(taskFont);
    setMinimumWidth(1200);
    setMinimumHeight(800);
    setWindowTitle("Изменения размера шрифта");
    setModal(true);
}

DialogFontSize::~DialogFontSize()
{
    delete engine;
    delete ui;
}

void DialogFontSize::accept()
{
    if (mathFont != curMathFont)
        emit changeMathFontSize(mathFont);
    if (taskFont != curTaskFont)
        emit changeTaskFontSize(taskFont);
    deleteLater();
}

void DialogFontSize::selectFontSize(QString font_size)
{
    QString tmp;
    if (font_size == "\\tiny") tmp = "Крошечный";
    else if (font_size == "\\Tiny") tmp = "Очень маленький";
    else if (font_size == "\\scriptsize") tmp = "Маленький";
    else if (font_size == "\\small") tmp = "Мелкий";
    else if (font_size == "\\normalsize") tmp = "Стандартный";
    else if (font_size == "\\large") tmp = "Крупный";
    else if (font_size == "\\Large") tmp = "Большой";
    else if (font_size == "\\LARGE") tmp = "Очень большой";
    else if (font_size == "\\huge") tmp = "Огромный";
    else tmp = "Невероятно огромный";

    for(int i = 0; i < ui->fontList->count(); ++i)
    {
        QListWidgetItem *item = ui->fontList->item(i);
        if (item->text() == tmp)  {
            ui->fontList->setCurrentItem(item);
            break;
        }
    }
}
