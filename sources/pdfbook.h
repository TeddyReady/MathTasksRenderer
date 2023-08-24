#ifndef PDFBOOK_H
#define PDFBOOK_H
#include "environment.h"

class PDFBook : public QWidget {
private:
    QLabel *icon, *label;
    QVBoxLayout *layout;
    bool selected;
public:
    explicit PDFBook(QString pdfFile = "default.pdf", QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // PDFBOOK_H
