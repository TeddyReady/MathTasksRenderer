#include "pdfviewer.h"

PDFViewer::PDFViewer(QString pdf_name, QWidget *parent)
    : pageNumber(0), fileName(pdf_name), QWidget(parent)
{
    if (fileName.split(QString(".")).last() != "pdf")
    {
        qDebug() << "Error with parsing non pdf file! File extension is "
                 << fileName.split(QString(".")).last();
        return;
    }

    uploadSettings();
    createWidget();
    parsePDF();
}

PDFViewer::~PDFViewer()
{
    saveSettings();
}

void PDFViewer::saveSettings()
{
    QSettings settings(App::OrgName, App::AppName);

    settings.beginGroup("Bibliography");
    settings.setValue(fileName, pageNumber);
    settings.endGroup();
}

void PDFViewer::uploadSettings()
{
    QSettings settings(App::OrgName, App::AppName);

    settings.beginGroup("Bibliography");
    pageNumber = settings.value(fileName).toInt();
    settings.endGroup();
}

void PDFViewer::createWidget()
{
    scene = new QGraphicsScene(this);
    prevBtn = new QPushButton(Symbols::arror_left, this);
    nextBtn = new QPushButton(Symbols::arror_right, this);

    prevBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    nextBtn->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QGraphicsView *view = new QGraphicsView(this);
    view->setScene(scene);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QGridLayout *btnLayout = new QGridLayout;

    mainLayout->addWidget(view, Qt::AlignCenter | Qt::AlignTop);
    btnLayout->addWidget(prevBtn, 1, 0, Qt::AlignLeft  | Qt::AlignBottom);
    btnLayout->addWidget(nextBtn, 1, 1, Qt::AlignRight | Qt::AlignBottom);

    mainLayout->addLayout(btnLayout);
    setLayout(mainLayout);

    connect(prevBtn, &QPushButton::clicked, this, &PDFViewer::prevPage);
    connect(nextBtn, &QPushButton::clicked, this, &PDFViewer::nextPage);
}

void PDFViewer::parsePDF()
{
    QFile pdfSource(fileName);
    if (!pdfSource.open(QFile::ReadOnly)) return;

    Poppler::Document *document = Poppler::Document::loadFromData(pdfSource.readAll());
    pdfSource.close();
    if (!document || document->isLocked())
    {
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

    if (pageNumber == 0) prevBtn->setDisabled(true);
    else if (pageNumber == images.size() - 1) nextBtn->setDisabled(true);

    scene->addPixmap(QPixmap::fromImage(images[pageNumber]));
    scene->setSceneRect(images[pageNumber].rect());
    delete document;
}

void PDFViewer::prevPage()
{
    if (!nextBtn->isEnabled()) nextBtn->setEnabled(true);

    if (pageNumber > 0)
    {
        scene->addPixmap(QPixmap::fromImage(images[--pageNumber]));
        if (pageNumber == 0)
            prevBtn->setDisabled(true);
    }
}

void PDFViewer::nextPage()
{
    if (!prevBtn->isEnabled()) prevBtn->setEnabled(true);

    if (pageNumber < images.size() - 1)
    {
        scene->addPixmap(QPixmap::fromImage(images[++pageNumber]));
        if (pageNumber == images.size() - 1)
            nextBtn->setDisabled(true);
    }
}
