#ifndef DEFINES_H
#define DEFINES_H
#include "common.h"

//App
namespace App {
    const QString AppName = QString("Algebra Madness");
    const QString OrgName = QString("Teddy's Corp");
}

//Resources
namespace RSC {
    const QString theoryPath = QString(":/theory/theory/theory.pdf");
    const QString checkAnswersPath = QString("://general/img/checkAnswers.png");
}

//MathJax
namespace MathJax {
    const QString startConf = QString("\\begin{aligned}\\color{sienna}{");
    const QString endConf = QString("}\\end{aligned}");
}

#endif // DEFINES_H
