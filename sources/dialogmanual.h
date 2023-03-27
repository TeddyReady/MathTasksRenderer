#ifndef DIALOGMANUAL_H
#define DIALOGMANUAL_H

#include "libs/mathlib/algebrastructures.h"
#include "texengine.h"

namespace Ui {
class DialogManual;
}

class DialogManual : public QDialog {
    Q_OBJECT
public:
    explicit DialogManual(QWidget *parent = nullptr);
    ~DialogManual();
private:
    TeXEngine *engine;
    Ui::DialogManual *ui;
};

#endif // DIALOGMANUAL_H
