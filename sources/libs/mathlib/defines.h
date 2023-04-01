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

//LaTeX
namespace LaTeX {
    const QString documentBegin = "\\documentclass[a4paper,12pt]{article}\n"
                                  "\\usepackage[T2A]{fontenc}\n"
                                  "\\usepackage{graphics}\n"
                                  "\\usepackage{amsfonts}\n"
                                  "\\usepackage{amsmath}\n"
                                  "\\usepackage{listings}\n"
                                  "\\usepackage[usenames,dvipsnames]{color}\n"
                                  "\\usepackage{graphicx}\n"
                                  "\\usepackage[english, russian]{babel}\n"
                                  "\\usepackage{tabularx}\n"
                                  "\\usepackage[left=20mm , top=20mm, right=20mm, bottom=20mm, nohead, nofoot]{geometry}\n"
                                  "\\title{\\textbf{WORKNAME}}\n"
                                  "\\author{\\textit{WORKTHEME}}\n"
                                  "\\date{WORKDATE}\n"
                                  "\\begin{document}\n"
                                  "\\maketitle\n";
    const QString documentEnd =   "\\end{document}";

    const QString tableBegin = "\\begin{array}{|r|l|}\\hline\n";
    const QString tableEnd =   "\\hline\\end{array}\n";
}

#endif // DEFINES_H
