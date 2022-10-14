#ifndef MAINTRANSLATORWIN_H
#define MAINTRANSLATORWIN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainTranslatorWin; }
QT_END_NAMESPACE

class MainTranslatorWin : public QMainWindow
{
    Q_OBJECT

public:
    MainTranslatorWin(QWidget *parent = nullptr);
    ~MainTranslatorWin();

private:
    Ui::MainTranslatorWin *ui;
};
#endif // MAINTRANSLATORWIN_H
