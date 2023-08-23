#ifndef PDFVIEWER_H
#define PDFVIEWER_H
#include "environment.h"

class PDFViewer : public QWidget {
    Q_OBJECT
private:
    QGraphicsScene *scene;
    QVector<QImage> images;
    QPushButton *prevBtn, *nextBtn;
    int pageNumber;
    QString fileName;

public:
    explicit PDFViewer(QString pdf_name, QWidget *parent = nullptr);
    ~PDFViewer();

private:
    void saveSettings();
    void uploadSettings();
    void createWidget();
    void parsePDF();

private slots:
    void prevPage();
    void nextPage();
};

#endif // PDFVIEWER_H
