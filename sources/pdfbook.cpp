#include "pdfbook.h"

PDFBook::PDFBook(QString pdfFile, QWidget *parent) : selected(false), QWidget(parent)
{
    icon = new QLabel;
    icon->setFixedSize(60, 80);
    icon->setPixmap(QPixmap(RSC::pics::pdf).scaled(icon->width(), icon->height()));
    icon->setAlignment(Qt::AlignHCenter);
    icon->setStyleSheet("background-color: none;");

    label = new QLabel(pdfFile.split(".").first());
    label->setAlignment(Qt::AlignHCenter);
    label->setStyleSheet("background-color: none;");

    layout = new QVBoxLayout(this);
    layout->addWidget(icon);
    layout->addWidget(label);

    setLayout(layout);
}

void PDFBook::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << event->x() << event->y();
    if (event->x() >= geometry().x() && event->x() <= geometry().width())
        setStyleSheet("background-color: none;");
    else
        setStyleSheet("background-color: tan;");
    event->accept();
}
