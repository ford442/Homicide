#include "csv.hpp"
#include <fstream>


using Doc = CSV::Document;
using N = CSV::Node;

N *create_node_from_str(std::string str);

Doc::Document(){

}

Doc::Document(std::string path){
    load(path);
}

Doc::~Document(){
    nodes.clear();

    if (is_printing_warnings && !errors.empty())
        std::cerr << "WARNING :: " << __func__ << " : " << errors.size() << " error(s) not readed" << std::endl;
    
    if (is_printing_warnings && !warnings.empty())
        std::cerr << "WARNING :: " << __func__ << " : " << warnings.size() << " warning(s) not readed" << std::endl;
    
    errors.clear();
    warnings.clear();
}

bool Doc::load(std::string path){
    this->path = path;
    return reload();
}

bool Doc::reload(void){
    std::ifstream file(path);

    if (!file.is_open()){
        errors.push_back("ERROR :: CSV::Document : cannot open \"" + path + "\" file");

        if (is_printing_errors)
            std::cerr << errors.back() << std::endl;

        return false;
    }

    int l=0;
    for (std::string line; std::getline(file, line);){
        l++;
        Node n;
        std::string err = n.load(line);

        if (err.empty()){
            nodes.push_back(n);
            continue;

        } else if (err[0] == 'W'){
            err.erase(0,1);
            warnings.push_back("WARNING :: CSV::Document : in file \"" + path + "\" line : " + std::to_string(l) + " : " + err);
            if (is_printing_warnings)
                std::cerr << warnings.back() << std::endl;

            nodes.push_back(n);
            continue;
        }

        errors.push_back("ERROR :: CSV::Cocument : in file \"" + path + "\" line : " + std::to_string(l) + " : " + err);
        if (is_printing_errors)
            std::cerr << errors.back() << std::endl;
    }

    file.close();
    return true;
}

void Doc::set_Debug(CSV_Debug_level dbg_level){
    switch (dbg_level){
        case CSV_Debug_none:
            set_custom_Debug(false, false, false);
            break;

        case CSV_Debug_1:
            set_custom_Debug(true, false, false);
            break;

        case CSV_Debug_2:
            set_custom_Debug(true, false, true);
            break;

        case CSV_Debug_3:
            set_custom_Debug(true, true, true);
            break;

        default:
            errors.push_back("ERROR :: CSV::Cocument : in " + std::string(__func__) + " cannot reconize input \"dbg_level\", uncknown value");
            if (is_printing_errors)
                std::cerr << errors.back() << std::endl;
            break;
    }
}

void Doc::set_custom_Debug(bool errors, bool logs, bool warnings){
    is_printing_errors = errors;
    is_printing_logs = logs;
    is_printing_warnings = warnings;
}

std::string Doc::search(std::string tag){
    for (auto &n : nodes){
        if (n.get_tag() == tag) return n.get_value();
    }
    return "";
}

std::list<N> Doc::get_nodes(){
    return nodes;
}


N::Node(){

}

N::Node(Document *doc){
    this->doc = doc;
}

N::~Node(){

}

std::string N::load(std::string str){
    std::size_t found = str.find(":");

    if (found == std::string::npos)
        return "cannot found ':' character, cannot found the separation between the tag and the value";

    for(unsigned int c=found+2; c<str.size(); c++){
        value += str[c];
    }

    for (unsigned int c=0; c<found; c++){
        tag += str[c];
    }

    for (unsigned int c=0; c<tag.size(); c++){
        if (tag[c] == ' '){
            tag.erase(c, 1);
        }
    }

    
    if (tag.empty()) return "cannot create a node with any name";
    if (value.empty()) return "Wnode \"" + tag + "\" dosen't have value";
    return "";
}

std::string N::get_tag(void) const{
    return tag;
}

std::string N::get_value(void) const{
    return value;
}

Doc *N::get_doc(void) const{
    return doc;
}

