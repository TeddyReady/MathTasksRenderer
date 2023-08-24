#ifndef LIBRARYWIDGET_H
#define LIBRARYWIDGET_H
#include "pdfbook.h"

class LibraryWidget : public QWidget {
    Q_OBJECT
private:
    QDir libraryDir;
    QVector<PDFBook> books;
public:
    explicit LibraryWidget(QWidget *parent = nullptr);

signals:

};

#endif // LIBRARYWIDGET_H
