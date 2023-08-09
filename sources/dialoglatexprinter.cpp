#include "dialoglatexprinter.h"
#include "ui_dialoglatexprinter.h"

DialogLatexPrinter::DialogLatexPrinter(const QVector<QString> &descriptions,
    const QVector<QVector<QString>> &tasks, const QVector<QString> &answers, QWidget *parent)
    : QDialog(parent), tasks(tasks), descriptions(descriptions), answers(answers), ui(new Ui::DialogLatexPrinter)
{
    ui->setupUi(this);
    ui->lineTitle->setText("Контрольная Работа №1");
    ui->lineTheme->setText("Функция Эйлера");
    ui->lineDate->setDate(QDate::currentDate());
    engine = new TeXEngine(ui->pdfView);
    engineAnswers = new TeXEngine(ui->pdfViewAnswers);

    //Create Tasks
    for (int i = 0; i < descriptions.size(); ++i)
    {
        tasksForView.append("{TASK_FONT\\textbf{" + descriptions.at(i) + ":}}\\\\");
        for (int j = 0; j < tasks.at(i).size(); ++j)
        {
            tasksForView.append("{MATH_FONT" + QString::number(j + 1) + ")~" + tasks.at(i).at(j) + "}\\\\");
        }
    }

    QSettings settings(App::OrgName, App::AppName);
    settings.beginGroup("Style");
    taskFontSize = settings.value("taskFontSize", "\\normalsize").toString();
    mathFontSize = settings.value("mathFontSize", "\\normalsize").toString();
    settings.endGroup();

    setMinimumWidth(1200);
    setMinimumHeight(800);
    setWindowTitle("Создание LaTeX & PDF файлов");
    setModal(true);
}

DialogLatexPrinter::~DialogLatexPrinter()
{
    delete engine;
    delete engineAnswers;
    delete ui;
}

void DialogLatexPrinter::printData()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохраните файл с заданиями", QDir::homePath(), "LaTeX & PDF файлы (*.tex, *.pdf)");

    QString title = "", theme = "", date = "";
    if (ui->btnDate->isChecked())
        date = ui->lineDate->text();
    if (ui->btnTheme->isChecked())
        theme = ui->lineTheme->text();
    if (ui->btnTitle->isChecked())
        title = ui->lineTitle->text();
    //TeX TASKS File
    if (!ui->btnLaTeX->isChecked()) return;
    QFile texFile(fileName + ".tex");
    if (!texFile.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Ошибка открытия файла", "Выбранный файл не может быть открыт или не существует.");
        return;
    } else {
        QString texString = "\\documentclass[12pt, a4paper]{article}\n"
                            "\\usepackage{graphicx}\n"
                            "\\usepackage[T2A]{fontenc}\n"
                            "\\usepackage[english, russian]{babel}\n"
                            "\\usepackage{amsmath}\n"
                            "\\usepackage{amsfonts}\n"
                            "\\title{" + title + "}\n"
                            "\\author{" + theme + "}\n"
                            "\\date{" + date + "}\n"
                            "\\begin{document}\n"
                            "\\maketitle\n"
                            "\\large{\n"
                            + tasksForView.replace("\\\\", "\\\\\n").replace("  ", "$").replace("?", "?$") +
                            "}\n"
                            "\\end{document}\n";
        QTextStream out(&texFile);
        out << texString;
        texFile.close();
    } close();
    // Answers file
    texFile.setFileName(fileName + "_answers.tex");
    if (!texFile.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Ошибка открытия файла", "Выбранный файл не может быть открыт или не существует.");
        return;
    } else {
        QTextStream out(&texFile);
        QString texString = "\\documentclass[12pt, a4paper]{article}\n"
                            "\\usepackage{graphicx}\n"
                            "\\usepackage[T2A]{fontenc}\n"
                            "\\usepackage[english, russian]{babel}\n"
                            "\\usepackage{amsmath}\n"
                            "\\usepackage{amsfonts}\n"
                            "\\title{" + title + "}\n"
                            "\\author{" + theme + "}\n"
                            "\\date{" + date + "}\n"
                            "\\begin{document}\n"
                            "\\begin{document}\n"
                            "\\maketitle\n"
                            "\\large{\n";
        out << texString + "}\n"
                                     "\\end{document}\n";
        texFile.close();
    }
}

void DialogLatexPrinter::showTasks()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QString TeXBuffer = tasksForView;
    TeXBuffer.replace("TASK_FONT", taskFontSize).replace("MATH_FONT", mathFontSize);

    if (ui->btnDate->isChecked())
        TeXBuffer += "{\\large\\text{" + ui->lineDate->text() + "}}\\\\" + TeXBuffer;

    if (ui->btnTheme->isChecked())
        TeXBuffer += "{\\LARGE\\textit{" + ui->lineTheme->text() + "}}\\\\" + TeXBuffer;

    if (ui->btnTitle->isChecked())
        TeXBuffer += "{\\huge\\textbf{" + ui->lineTitle->text() + "}}\\\\" + TeXBuffer;

    engine->TeX2SVG(MathJax::startConf + TeXBuffer + MathJax::endConf, true);
    QApplication::restoreOverrideCursor();
}

void DialogLatexPrinter::showAnswers()
{
}

void DialogLatexPrinter::on_btnShow_clicked()
{
    showTasks();
    showAnswers();
}

void DialogLatexPrinter::on_buttonBox_accepted()
{
    printData();
    close();
}


void DialogLatexPrinter::on_buttonBox_rejected()
{
    close();
}

