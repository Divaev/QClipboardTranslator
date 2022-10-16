#ifndef WORDS_FINDER_H
#define WORDS_FINDER_H
#include "pugixml.hpp"
#include <iostream>
#include <QMap>
#include <QPair>


#define MAX_WORD_SIZE 128


class WordsFinder {


    const char* current_word;
    pugi::xml_document current_dictionary;


public:

    class FinderError;

    WordsFinder();

    pugi::xml_node findTheSingleWorld(const char* word) const;

    //const char* translateTheSingleWorld(const pugi::xml_node&) const;

    QMap<QString, QString> translateTheSingleWorld(const pugi::xml_node&) const;

    bool initTheDict(const char* dict_path);

    ~WordsFinder();

};

class WordsFinder::FinderError {
    const char* errorMsg;
public:
    FinderError(const char* err);
    void typeError() const;
    const char* getError() const;
};

#endif // WORDS_FINDER_H
