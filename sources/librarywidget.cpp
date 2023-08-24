#include "librarywidget.h"

LibraryWidget::LibraryWidget(QWidget *parent)
    : libraryDir(App::LibPath), QWidget(parent)
{
    QGridLayout *lay = new QGridLayout(this);
    foreach(QFileInfo item, libraryDir.entryInfoList() )
    {
        if (item.isFile())
        {
            PDFBook book(item.fileName());
//            books.push_back(book);
            lay->addWidget(&book, 0, 0, Qt::AlignCenter);
            qDebug() << "Book created: " << item.fileName();
        }
    }

    setLayout(lay);
}
