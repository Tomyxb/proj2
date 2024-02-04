
#include "DSVReader.h"
#include <sstream>
#include <iostream>

struct CDSVReader::SImplementation {
    std::shared_ptr<CDataSource> Source;
    char Delimiter;
    std::string line_now;
    bool IsEnd;

    SImplementation(std::shared_ptr<CDataSource> src, char delimiter)
        : Source(src), Delimiter(delimiter), IsEnd(false){}

    //read line
    bool Readline(){
        if (IsEnd || !Source || !std::getline(*Source, line_now)) {
            IsEnd=true;
            return false;
        }
        return true;
    }
};

//construction
CDSVReader::CDSVReader(std::shared_ptr<CDataSource> src, char delimiter)
    : DImplementation(new SImplementation(src, delimiter)){}

//no need to put anything in
CDSVReader::~CDSVReader() {}

//end test
bool CDSVReader::End() const{
    return DImplementation->IsEnd;
}

//separeted read line to parts in a vector
bool CDSVReader::ReadRow(std::vector<std::string> &row){
    //test if input ends
    if (!DImplementation->Readline()){
        return false;
    }
    //get the line
    std::stringstream line_in(DImplementation->line_now);
    //store for words
    std::string words_spareted;
    //push every words in the final result
    while (std::getline(line_in, words_spareted, DImplementation->Delimiter)){
        row.push_back(words_spareted);
    }

    return true;
}

