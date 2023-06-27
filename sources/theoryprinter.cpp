#include "generatorwindow.h"
#include "ui_generatorwindow.h"

void GeneratorWindow::createTheoryImages()
{
    QFile pdfSource(RSC::theoryPath);
    if (!pdfSource.open(QFile::ReadOnly)) return;
    Poppler::Document *document = Poppler::Document::loadFromData(pdfSource.readAll());
    pdfSource.close();
    if (!document || document->isLocked()) {
      qDebug() << "ERROR WITH POPPLER";
      delete document;
      return;
    }
    document->setRenderHint(Poppler::Document::TextAntialiasing);
    Poppler::Page* pdfPage; QImage image;
    for (int i = 0; i < document->numPages(); ++i)
    {
        pdfPage = document->page(i);
        image = pdfPage->renderToImage(135.0, 135.0);
        images.push_back(image);
        delete pdfPage;
    }

    if (pageNumber == 0) ui->prevPage->setDisabled(true);
    else if (pageNumber == images.size() - 1) ui->nextPage->setDisabled(true);

    scene->addPixmap(QPixmap::fromImage(images[pageNumber]));
    scene->setSceneRect(images[pageNumber].rect());
    delete document;
}

void GeneratorWindow::prevTheoryPage()
{
    if (!ui->nextPage->isEnabled()) ui->nextPage->setEnabled(true);

    if (pageNumber > 0) {
        scene->addPixmap(QPixmap::fromImage(images[--pageNumber]));
        if (pageNumber == 0)
            ui->prevPage->setDisabled(true);
    }
}

void GeneratorWindow::nextTheoryPage()
{
    if (!ui->prevPage->isEnabled()) ui->prevPage->setEnabled(true);

    if (pageNumber < images.size() - 1) {
        scene->addPixmap(QPixmap::fromImage(images[++pageNumber]));
        if (pageNumber == images.size() - 1)
            ui->nextPage->setDisabled(true);
    }
}
