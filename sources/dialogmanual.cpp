#include "dialogmanual.h"
#include "ui_dialogmanual.h"

DialogManual::DialogManual(QString taskFontSize, QString mathFontSize, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogManual)
{
    ui->setupUi(this);
    engine = new TeXEngine(ui->webView);
    ui->label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QDomDocument document;
    QFile xmlFile(RSC::data::algebra_structures);
    if (!xmlFile.open(QIODevice::ReadOnly ))
    {
       qDebug() << "Cannot open xml file:" << RSC::data::algebra_structures;
       return;
    }
    document.setContent(&xmlFile);
    xmlFile.close();

    QDomNodeList dataSet = document.elementsByTagName(QString("set"));

    QString info;
    for (int setCounter = 0; setCounter < dataSet.count(); ++setCounter)
    {
        if (dataSet.item(setCounter).isElement())
        {
            QDomElement description = dataSet.item(setCounter).firstChildElement();
            info += QString("%1:%2").arg(dataSet.item(setCounter).toElement().attribute("name")).arg(description.text().replace(" ", "~")) + "\\\\";
        }
    }
    engine->TeX2SVG(MathJax::startConf + info + MathJax::endConf, true);
}

DialogManual::~DialogManual()
{
    delete engine;
    delete ui;
}
