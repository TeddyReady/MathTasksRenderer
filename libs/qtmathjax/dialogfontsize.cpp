#include "dialogfontsize.h"
#include "ui_dialogfontsize.h"

DialogFontSize::DialogFontSize(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFontSize)
{
    ui->setupUi(this);
}

DialogFontSize::~DialogFontSize()
{
    delete ui;
}
