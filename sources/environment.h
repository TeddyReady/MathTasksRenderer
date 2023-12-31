#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include "include.h"

/* NAMESPACES */

/* App */
namespace App {
    const QString AppName = QString("Algebra Madness");
    const QString OrgName = QString("Teddy's Corp");
}

/* Resources */
namespace RSC {
    namespace data {
        const QString algebra_structures= QString(":/data/data/algebra_structures.xml");
    }

    namespace general {
        const QString style             = QString(":/general/style.css");
    }

    namespace pics {
        const QString appIcon           = QString(":/pics/pics/appIcon.png");
        const QString checkAnswers      = QString(":/pics/pics/checkAnswers.png");
        const QString checked           = QString(":/pics/pics/checked.png");
        const QString checked_hover     = QString(":/pics/pics/checked_hover.png");
        const QString clearTasks        = QString(":/pics/pics/clearTasks.png");
        const QString font_size         = QString(":/pics/pics/font-size.png");
        const QString manual            = QString(":/pics/pics/manual.png");
        const QString printer           = QString(":/pics/pics/printer.png");
        const QString quit              = QString(":/pics/pics/quit.png");
        const QString reload            = QString(":/pics/pics/reload.png");
        const QString splash            = QString(":/pics/pics/splash.png");
        const QString unchecked_hover   = QString(":/pics/pics/unchecked_hover.png");
        const QString compile           = QString(":/pics/pics/compile.png");
        const QString reference_manual  = QString(":/pics/pics/reference_manual.png");
        const QString listWidget_arrow  = QString(":/pics/pics/listWidget_arrow.png");
    }

    namespace theory {
        const QString theory            = QString(":/theory/theory/theory.pdf");
        const QString MNR               = QString(":/theory/theory/turing-machine_reference-manual.pdf");
    }

    namespace xml {
        const QString tasks             = QString(":/xml/xml/tasks.xml");
    }
}

/* MathJax */
namespace MathJax {
    const QString startConf = QString("\\begin{aligned}\\color{sienna}{");
    const QString endConf   = QString("}\\end{aligned}");
}

/* LaTeX */
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

    const QString tableBegin =    "\\begin{array}{|r|l|}\\hline\n";
    const QString tableEnd =      "\\hline\\end{array}\n";
}

namespace Symbols {
    const QString arror_left  = "\u2190";
    const QString arror_right = "\u2192";
}

/* ENUM */

/* All task types in Algebra Madness */
enum class AllTasks {
    EulerFunction       ,
    MebiusFunction      ,
    SymbolLegandre      ,
    SymbolJacobi        ,
    TranspositionGroup  ,
    Matrix              ,
    AlgebraStructures   ,
    GroupProperties     ,
    RingResidue         ,
    RingOfMembers       ,
    Complex             ,
    BooleanFunction     ,
    KeliTable           ,
};

enum class ExoticOption {
    None            ,
    Transposition   ,
    MatrixDefault   ,
    MatrixInverse   ,
    ResidueDefault  ,
    ResidueGroups   ,
    ResidueAll      ,
    MembersAll      ,
    MembersFields   ,
    Complex         ,
};

/* Main ToolBar Structure */
enum ToolBarOptions {
    Check   ,
    Clear   ,
    Sep_1   ,
    Print   ,
    Sep_2   ,
    Compile ,
    MNRInfo ,
    Spacer  ,
    Manual  ,
    Font    ,
    Exit    ,
};

enum class TabBarModes {
    Tasks           ,
    Library         ,
    TestMode        ,
    TuringMachine   ,
};

/* Useful algebra sets */
enum class Set {
    N,  No, Z,  Z0,
    Q,  Q0, R,  R0,
    C,  Sn, Dn, An,
    V4, O8, Zn, Mm_Z,
    Mm_R, Mn_Z, Mn_R,
    _2A, Func_C, Func_D,
    Pn_X, ABS_C_, V_2D,
    V_nD, Zp, Z_i, MultiGroup_Zn
};

#endif // ENVIRONMENT_H
