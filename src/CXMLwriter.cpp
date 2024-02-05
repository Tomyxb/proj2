#include "XMLWriter.h"
#include "StringDataSink.h" 
#include <sstream>
#include <vector>

// Define the private implementation for CXMLWriter.
struct CXMLWriter::SImplementation {
    // Shared pointer to a data sink object for output.
    std::shared_ptr<CDataSink> DSink;

    // Constructor initializes the data sink.
    SImplementation(std::shared_ptr<CDataSink> sink) : DSink(sink) {}

    // Write function converts string data to a vector of chars and writes it to the data sink.
    bool Write(const std::string& data) {
        // Convert string data to a vector of chars.
        std::vector<char> buffer(data.begin(), data.end());
        // Write the data through the data sink interface.
        return DSink->Write(buffer);
    }
};

// Constructor for CXMLWriter initializes the private implementation with the provided data sink.
CXMLWriter::CXMLWriter(std::shared_ptr<CDataSink> sink)
    : DImplementation(new SImplementation(sink)) {
}

// Destructor for CXMLWriter. It's empty but necessary for proper cleanup.
CXMLWriter::~CXMLWriter() {
}

bool CXMLWriter::Flush() {
    return true;
}

// WriteEntity function constructs XML from the provided SXMLEntity object and writes it using the Write function.
bool CXMLWriter::WriteEntity(const SXMLEntity &entity) {
    // Use a string stream to build the XML string.
    std::ostringstream oss;

    // Depending on the type of the XML entity, construct the appropriate XML string.
    switch (entity.DType) {
    case SXMLEntity::EType::StartElement: {
        // Handle start elements
        oss << "<" << entity.DNameData;
        for (const auto& attr : entity.DAttributes) {
            oss << " " << attr.first << "=\"" << attr.second << "\"";
        }
        oss << ">";
        break;
    }
    case SXMLEntity::EType::EndElement: {
        // Handle end elements
        oss << "</" << entity.DNameData << ">";
        break;
    }
    case SXMLEntity::EType::CompleteElement: {
        // Handle complete elements (self-closing tags)
        oss << "<" << entity.DNameData;
        for (const auto& attr : entity.DAttributes) {
            oss << " " << attr.first << "=\"" << attr.second << "\"";
        }
        oss << "/>";
        break;
    }
    default: {
        
        break;
    }
}

    

    return DImplementation->Write(oss.str());
}
