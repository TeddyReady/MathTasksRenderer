#include "librarywidget.h"

LibraryWidget::LibraryWidget(QWidget *parent)
    : libraryDir(App::LibPath), QWidget(parent)
{
    int rowCounter = 0, colCounter = 0;

    QGridLayout *lay = new QGridLayout(this);
    foreach(QFileInfo item, libraryDir.entryInfoList() )
    {
        if (item.isFile())
        {
            lay->addWidget(new PDFBook(item.fileName()), rowCounter, colCounter, Qt::AlignTop);
            ++colCounter;
        }

        if (colCounter == 8)
        {
            colCounter = 0;
            ++rowCounter;
        }
    }

    setLayout(lay);
    show();
}
