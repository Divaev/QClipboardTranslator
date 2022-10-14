#include "includes/maintranslatorwin.h"

#include <QApplication>

#include <clipboardchkr.h>

int main(int argc, char *argv[])
{
    ClipboardChecker clipChkr;
    clipChkr.testXML();
    QApplication a(argc, argv);
    MainTranslatorWin w;
    w.show();
    return a.exec();
}
