#include "DSVWriter.h"
#include <sstream>
#include <iostream>

class CDSVWriter::SImplementation {
public:
    std::shared_ptr<CDataSink> Sink;
    char Delimiter;
    bool QuoteAll;//if quote is needed

    SImplementation(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
        : Sink(sink), Delimiter(delimiter), QuoteAll(quoteall) {}

    //for special symbols
   std::string EscapeString(const std::string &input) {
    std::string output;
    bool needQuotes = input.find(Delimiter) != std::string::npos ||
                      input.find('\n') != std::string::npos ||
                      input.find('"') != std::string::npos;
    if (needQuotes) {
        output += '"';
    }
    for (auto ch : input) {
        if (ch == '"') {
            output += "\"\"";
        } else {
            output += ch;
        }
    }
    if (needQuotes) {
        output += '"';
    }
    return output;
    }
    
    bool WriteRowImpl(const std::vector<std::string> &row) {
        std::stringstream ss;
        for (size_t i = 0; i < row.size(); ++i) {
            if (i > 0) {
                ss << Delimiter;
            }

            std::string cell = row[i];
            if (QuoteAll || cell.find(Delimiter) != std::string::npos || 
                cell.find('"') != std::string::npos || cell.find('\n') != std::string::npos) {
                cell = '"' + EscapeString(cell) + '"';
            }
            ss << cell;
        }
        ss << std::endl;
        std::string str = ss.str();
        std::vector<char> vec(str.begin(), str.end());
        return Sink->Write(vec);
    }
};


CDSVWriter::CDSVWriter(std::shared_ptr<CDataSink> sink, char delimiter, bool quoteall)
    : DImplementation(new SImplementation(sink, delimiter == '"' ? ',' : delimiter, quoteall)) {}

//no space needs extra free
CDSVWriter::~CDSVWriter() {}

//output
bool CDSVWriter::WriteRow(const std::vector<std::string> &row) {
    return DImplementation->WriteRowImpl(row);
}


