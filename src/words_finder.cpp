#include "words_finder.h"
#include <cstring>

WordsReceiver::WordsReceiver(QObject* parent): QObject(parent) {

}

/*
void WordsReceiver::receiveFromClipboard(const QString& input_data) {
    emit dataIsReceived(input_data);
}
*/

void WordsReceiver::setCurrentWord(const QString& word) {
    input_word = word;
}


/*
pugi::xml_node WordsFinder::findTheSingleWorld(const char* word) const {
    pugi::xml_node main_node = current_dictionary.child("xdxf");
    pugi::xml_node found_node = current_dictionary.find_node([word](const pugi::xml_node& curr_node) {
                                                                    return strcmp(curr_node.value(), word) ? false: true;
                                                                });

    return found_node;
}
*/


void WordsFinder::findTheSingleWord(const QString& word) {
    pugi::xml_node main_node = current_dictionary.child("xdxf");
    pugi::xml_node found_node = current_dictionary.find_node([word](const pugi::xml_node& curr_node) {
                                                                    return strcmp(curr_node.value(), word.toStdString().c_str()) ? false: true;
                                                                });

    if(found_node != nullptr) {
        QMap<QString, QString> result = translateTheSingleWorld(found_node);
        emit translationIsReady(result);
    }

}


QMap<QString, QString> WordsFinder::translateTheSingleWorld(const pugi::xml_node& found_node) const {
    std::cout << "Source word: " << found_node.value() << std::endl;
    auto node = found_node.parent();
    node = node.next_sibling();

    QMap<QString, QString> translationResult;

    while(node != nullptr) {

        if(node.type() == pugi::xml_node_type::node_element) {
            std::cout << "Transcription: " << node.child_value() << std::endl;
            translationResult["transcription"] = node.child_value();
        }
        else if(node.type() == pugi::xml_node_type::node_pcdata) {
            std::cout << "Translation: " << node.value() << std::endl;
            translationResult["translation"] = node.value();
        }
        node = node.next_sibling();
    }

    return translationResult;
}

void WordsFinder::initTheDict(const QString& dict_path) {
    current_dictionary.load_file(dict_path.toStdString().c_str());
}


WordsFinder::FinderError::FinderError(const char* err): errorMsg(err) {

}

void WordsFinder::FinderError::typeError() const {
    std::cout << errorMsg << std::endl;
}

const char *WordsFinder::FinderError::getError() const {
    return errorMsg;
}

WordsFinder::~WordsFinder() {

}

WordsFinderThread::WordsFinderThread(WordsReceiver *receiver,
                                     const QString& _dict_path,
                                     QObject* parent): wordsReceiver(receiver),
                                                       dict_path(_dict_path),
                                                       QThread(parent) {
    qDebug() << "wordsFinderThread constructor";
}

void WordsFinderThread::run() {

    wordsFinder = new WordsFinder;
    wordsFinder->initTheDict(dict_path);

    QObject::connect(wordsReceiver, &WordsReceiver::wordIsReceived,
                     wordsFinder, &WordsFinder::findTheSingleWord);

    QObject::connect(wordsFinder, &WordsFinder::translationIsReady,
                     wordsReceiver, &WordsReceiver::sendResultOut);

    exec();
}

WordsFinder* WordsFinderThread::getWordsFinder() {
    return wordsFinder;
}

