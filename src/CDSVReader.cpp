
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
            : DImplementation(new SImplementation(src, delimiter == '"' ? ',' : delimiter)) {}//change '"'to ,
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

    std::stringstream line_in(DImplementation->line_now);
    std::string word;
    bool inside_quotes = false;//test if inside the quotes
    char next_char;

    while (line_in >> std::noskipws >> next_char) {
        if (next_char == '"'){
            inside_quotes = !inside_quotes; //change the state of if inside the quotes
            if (inside_quotes && line_in.peek() == '"'){"s"
                //if '"' inside, add it to word
                line_in >> next_char;
                word += next_char;
            }
        } else if (next_char == DImplementation->Delimiter && !inside_quotes){
            //if not inside, end
            row.push_back(word);
            word.clear();
        } else if (next_char == '\n' && !inside_quotes){
            //if not inside and no more words in this line
            break;
        } else {
            word += next_char;//no quotes
        }
    }
    if (!word.empty() || DImplementation->line_now.back() == DImplementation->Delimiter) {
        //ensure the empty line would be added
        row.push_back(word);
    }

    return true;