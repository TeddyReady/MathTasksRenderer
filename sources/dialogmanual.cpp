#include "dialogmanual.h"
#include "ui_dialogmanual.h"

DialogManual::DialogManual(QString taskFontSize, QString mathFontSize, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogManual)
{
    ui->setupUi(this);
    engine = new TeXEngine(ui->webView);
    ui->label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    AlgebraStructures AS;
    QString result;
    for (int i = 0; i < AS.getSetsCount(); ++i)
    {
        result.append("{" + mathFontSize + AS.getCode(static_cast<Set>(i)) + "}\\Rightarrow{" +
                      taskFontSize + AS.getSetName(static_cast<Set>(i)) + "}\\\\");
    }
    engine->TeX2SVG("\\begin{aligned}\\color{sienna}{" + result + "}\\end{aligned}", true);

    connect(ui->btnExit, &QPushButton::clicked, [&](){ deleteLater(); });
    connect(ui->btnReload, &QPushButton::clicked, [=](){
        engine->TeX2SVG("\\begin{aligned}\\color{sienna}{" + result + "}\\end{aligned}", true);
    });

    setMinimumWidth(1280);
    setMinimumHeight(720);
    setWindowTitle("Руководство по условным обозначениям");
}

DialogManual::~DialogManual()
{
    delete engine;
    delete ui;
}
