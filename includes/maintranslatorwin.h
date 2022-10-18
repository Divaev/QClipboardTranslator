#ifndef MAINTRANSLATORWIN_H
#define MAINTRANSLATORWIN_H

#include <QMainWindow>
#include "words_finder.h"
#include "clipboardchkr.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainTranslatorWin; }
QT_END_NAMESPACE

class MainTranslatorWin : public QMainWindow
{
    Q_OBJECT

    WordsFinder translator;
    QString current_word;
    ClipboardChecker clipboardChecker;

public:
    MainTranslatorWin(QWidget *parent = nullptr);
    ~MainTranslatorWin();

private:
    Ui::MainTranslatorWin *ui;
    void translateInputWord(const pugi::xml_node& found_node);

public slots:

    void findInputWord();
    void makeTranslateAvailable();

signals:
    void wordIsNotFound();
};
#endif // MAINTRANSLATORWIN_H
