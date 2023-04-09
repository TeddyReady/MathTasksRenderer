#ifndef DIALOGFONTSIZE_H
#define DIALOGFONTSIZE_H

#include "libs/mathlib/environment.h"
#include "texengine.h"

namespace Ui {
class DialogFontSize;
}

class DialogFontSize : public QDialog {
    Q_OBJECT
public:
    explicit DialogFontSize(QString curTaskFont, QString curMathFont, QWidget *parent = nullptr);
    ~DialogFontSize();
signals:
    void changeTaskFontSize(QString font_size);
    void changeMathFontSize(QString font_size);
protected:
    void closeEvent(QCloseEvent *event);
    void accept();
private:
    void saveSettings();
    void uploadSettings();
    void selectFontSize(QString font_size);

    bool isIgnoring;
    QString mainPage, taskFont, mathFont;
    QString curTaskFont, curMathFont;
    TeXEngine *engine;
    Ui::DialogFontSize *ui;
};

#endif // DIALOGFONTSIZE_H
