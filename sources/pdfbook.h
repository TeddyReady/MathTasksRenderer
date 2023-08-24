#ifndef PDFBOOK_H
#define PDFBOOK_H
#include "environment.h"

class PDFBook : public QWidget {
private:
    QLabel *icon, *label;
    QGridLayout *layout;
public:
    explicit PDFBook(QString pdfFile = "default.pdf", QWidget *parent = nullptr);
};

#endif // PDFBOOK_H
