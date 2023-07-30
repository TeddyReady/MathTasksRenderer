#ifndef DIALOGMANUAL_H
#define DIALOGMANUAL_H
#include "texengine.h"
#include "include.h"

namespace Ui {
class DialogManual;
}

class DialogManual : public QDialog {
    Q_OBJECT
public:
    explicit DialogManual(QString taskFontSize, QString mathFontSize, QWidget *parent = nullptr);
    ~DialogManual();
private:
    TeXEngine *engine;
    Ui::DialogManual *ui;
};

#endif // DIALOGMANUAL_H
