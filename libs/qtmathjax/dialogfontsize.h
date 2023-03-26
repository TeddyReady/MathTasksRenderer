#ifndef DIALOGFONTSIZE_H
#define DIALOGFONTSIZE_H

#include <QDialog>

namespace Ui {
class DialogFontSize;
}

class DialogFontSize : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFontSize(QWidget *parent = nullptr);
    ~DialogFontSize();

private:
    Ui::DialogFontSize *ui;
};

#endif // DIALOGFONTSIZE_H
