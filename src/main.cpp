#include "includes/maintranslatorwin.h"

#include <QApplication>

#include <clipboardchkr.h>

#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
    //const char *old_style_string = new char[10];

    /*
    char *const old_style_string = new char[10]; //This is constant pointer
    const char *old_style_string_1 = "Hello world!";
    std::strcpy(old_style_string, "hello");
    std::cout << old_style_string << std::endl;
    std::cout << old_style_string_1 << std::endl;
    */


    ClipboardChecker clipChkr;
    clipChkr.testXML();
    QApplication a(argc, argv);
    MainTranslatorWin w;
    w.show();
    return a.exec();
}
