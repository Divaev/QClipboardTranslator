#ifndef CLIPBOARDCHKR_H
#define CLIPBOARDCHKR_H
#include <QClipboard>
#include <QApplication>
#include <QDebug>

#include <thread>
#include <chrono>


class ClipboardChecker {
    QClipboard* clipboard;
public:
    ClipboardChecker();
    int testXML() const;
    void testClipboard() const;
};

#endif // CLIPBOARDCHKR_H
