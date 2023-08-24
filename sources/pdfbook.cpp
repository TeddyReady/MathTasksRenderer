#include "pdfbook.h"

PDFBook::PDFBook(QString pdfFile, QWidget *parent) : QWidget(parent)
{
    icon = new QLabel;
    icon->setFixedSize(120, 120);
    icon->setPixmap(QPixmap(RSC::pics::pdf).scaled(icon->width(), icon->height()));

//    label = new QLabel(pdfFile.split(".").first());
//    label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    layout = new QGridLayout(this);
    layout->addWidget(icon, 0, 0, Qt::AlignCenter);
//    layout->addWidget(label, Qt::AlignCenter);

    setLayout(layout);
//    setFixedSize(icon->width(), icon->height());
    show();

}
