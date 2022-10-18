#include "clipboardchkr.h"
#include <iostream>
#include <cstring>
#include "pugixml.hpp"

ClipboardChecker::ClipboardChecker(): clipboard(QApplication::clipboard()) {

}

int ClipboardChecker::testXML() const {

    std::cout << "---------------Test note output: ----------------------------" << std::endl;
    pugi::xml_document test_note;
    //pugi::xml_parse_result result = doc.load_file("/home/ingenigger/Projects/QClipboardTranslator/dict.xdxf");
    pugi::xml_parse_result test_result = test_note.load_file("note.xml");
    if (!test_result) {
        std::cout << "Can not open .xml" << std::endl;
        return -1;
    }
    else {
        std::cout << "It's OK" << std::endl;
        std::cout << "Info: " << test_result.description() << std::endl;
    }
    pugi::xml_node my_node_1 = test_note.child("note");                 //pugi::xml_node::child(const char* name) method finds the node by its tag
    if(my_node_1 != nullptr) {
        std::cout << "my_node_1 is detected!" << std::endl;
        std::cout << "name of my_node_1: " << my_node_1.name() << std::endl;
        std::cout << (my_node_1.value() ? "my_node_1 has a value": "my_node_1 doesn't have a value") << std::endl;
        std::cout << "value of my_node_1: " << my_node_1.value() << std::endl;
        std::cout << (strcmp(my_node_1.value(), "/n") ? "my_node_1 value is empty!": "my_node_1 is NOT empty") << std::endl;  //node element(tag) always has a name but never has a value
        std::cout << "child_value: " << my_node_1.child_value() << std::endl;           //The data of the node is the child content of the node too!

        for(pugi::xml_node curr_child = test_note.child("note"); curr_child; curr_child = curr_child.next_sibling()) {      //The first node with the "note" tag was found
            std::cout << "curr_child value: " << curr_child.child_value() << std::endl;                                     //Then we started to iterate through all its neighbours
        }

        pugi::xml_node the_main_parent_node = test_note.child("note");
        auto curr_node = the_main_parent_node.first_child();

        std::cout << "Going throug all \"note\" childs: " << std::endl;
        while(curr_node != nullptr) {
            if(curr_node.type() == 3)               //Depending on the node type we type either node name or node value
                std::cout << curr_node.value();
            else if(curr_node.type() == 2)
                std::cout << curr_node.name() << std::endl;

            curr_node = curr_node.next_sibling();
        }
    }

    else
        std::cout << "no my_node_1" << std::endl;





    for (pugi::xml_node tool: test_note.child("Profile").child("Tools").children("Tool")) {                           //child(const char *name) returns the first node with needed name
        int timeout = tool.attribute("Timeout").as_int();

        if (timeout > 0)
            std::cout << "Tool " << tool.attribute("Filename").value() << " has timeout " << timeout << "\n";
     }

    for (pugi::xml_node word: test_note.child("Hello")) {
        std::cout << "word is found" << std::endl;
    }
    for (pugi::xml_node word: test_note.child("to")) {
        std::cout << "word is found" << std::endl;
    }


    std::cout << "---------------Dictionary output: ----------------------------" << std::endl;
    pugi::xml_document doc;
    //pugi::xml_parse_result result = doc.load_file("/home/ingenigger/Projects/QClipboardTranslator/dict.xdxf");
    pugi::xml_parse_result result = doc.load_file("dict.xdxf");         //Loading the .xdxf dictionary from the text file

    pugi::xml_node word = doc.child("xdxf");
    pugi::xml_node found_node = word.find_node([](const pugi::xml_node &curr_node){if (!strcmp(curr_node.value(), "final reminder"))
                                                        return true;
                                                       else
                                                        return false;
                                                        });
    std::cout << "Found_node value: " << found_node.value() << std::endl;
    std::cout << "Found_node name: " << found_node.parent().name() << std::endl;


    std::cout << "doc.child_value(): " << doc.child_value() << std::endl;
    std::cout << "doc.first_child(): " << doc.first_child().first_child().value() << std::endl;

    for (pugi::xml_node word: doc.child("note")) {
        std::cout << "word is found" << std::endl;
        //std::cout << word.attribute("to").value() << std::endl;
        //std::cout << word.name() << std::endl;
        auto curr_node = word.first_child();
        if(curr_node == nullptr) {
            std::cout << "null node was detected!" << std::endl;
        }
        std::cout << "word.value(): " << word.value() << std::endl;
        std::cout << "First child value: " << word.first_child().value() << std::endl;
        std::cout << "Last child value: " << word.last_child().first_child().value() << std::endl;
        std::cout << "Next_sibling: " << word.next_sibling().first_child().value() << std::endl;
        std::cout << "word.child_value(): " << word.child_value() << std::endl;

    }

//    std::cout << "first child of doc: " << doc.first_child().value() << std::endl;


    for (pugi::xml_node word: doc.child("name")) {
        std::cout << "name word is found" << std::endl;
    }
    for (pugi::xml_node word: doc.child("Reminder")) {
        std::cout << "Reminder word is found" << std::endl;
    }
    for (pugi::xml_node word: doc.child("name")) {
        std::cout << "name word is found" << std::endl;
    }

    std::cout << "End" << std::endl;

    return 0;
}

void ClipboardChecker::testClipboard() const {

    while(true) {
        static int counter;

        clipboard->setText("lol1" + QString::number(counter), QClipboard::Clipboard);

        if (clipboard->supportsSelection()) {
            clipboard->setText("lol1" + QString::number(counter), QClipboard::Selection);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(clipboard != nullptr)
            qDebug() << clipboard->text();

        ++counter;
        counter %= 10;

    }

}
