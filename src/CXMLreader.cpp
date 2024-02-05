#include "XMLReader.h"
#include <expat.h>
#include <vector>
#include <stdexcept>

// Private implementation structure for CXMLReader.
struct CXMLReader::SImplementation {
    std::shared_ptr<CDataSource> DataSource; // Shared pointer to the data source object.
    XML_Parser Parser; // Expat parser instance for parsing XML data.
    bool EndOfData; // Flag to indicate if the end of the data source has been reached.
    SXMLEntity CurrentEntity; // The current XML entity being processed.
    bool IsEntityReady; // Flag to indicate if an XML entity is ready for retrieval.

    // Constructor initializes the parser and sets up callback functions.
    SImplementation(std::shared_ptr<CDataSource> src) : DataSource(src), EndOfData(false), IsEntityReady(false) {
        Parser = XML_ParserCreate(nullptr);
        if (!Parser) {
            throw std::runtime_error("Failed to create XML parser");
        }
        XML_SetUserData(Parser, this); // Associate this instance as user data for callbacks.
        XML_SetElementHandler(Parser, StartElementHandler, EndElementHandler); // Set handlers for start and end tags.
        XML_SetCharacterDataHandler(Parser, CharDataHandler); // Set handler for character data.
    }

    // Destructor frees the parser to prevent memory leaks.
    ~SImplementation() {
        XML_ParserFree(Parser);
    }

    // Callback function for start element tags.
    static void StartElementHandler(void *userData, const char *name, const char **atts) {
        auto *impl = static_cast<SImplementation*>(userData);
        impl->CurrentEntity = SXMLEntity();
        impl->CurrentEntity.DType = SXMLEntity::EType::StartElement;
        impl->CurrentEntity.DNameData = name;
        // Parse attributes into the entity.
        while (*atts) {
            const char* attrName = *atts++; // Read name and move to the next attribute
            const char* attrValue = *atts++; // Read value and move to the next attribute,

        
            impl->CurrentEntity.DAttributes.emplace_back(attrName, attrValue);;
        }
        impl->IsEntityReady = true; // Mark entity as ready for retrieval.
    }

    // Callback function for end element tags.
    static void EndElementHandler(void *userData, const char *name) {
        auto *impl = static_cast<SImplementation*>(userData);
        impl->CurrentEntity = SXMLEntity();
        impl->CurrentEntity.DType = SXMLEntity::EType::EndElement;
        impl->CurrentEntity.DNameData = name;
        impl->IsEntityReady = true; // Mark entity as ready for retrieval.
    }

    // Callback function for character data.
    static void CharDataHandler(void *userData, const char *s, int len) {
        auto *impl = static_cast<SImplementation*>(userData);
        // Ignore purely whitespace text nodes.
        std::string data(s, len);
        if (data.find_first_not_of(" \t\n\r") != std::string::npos) {
            impl->CurrentEntity = SXMLEntity();
            impl->CurrentEntity.DType = SXMLEntity::EType::CharData;
            impl->CurrentEntity.DNameData = data;
            impl->IsEntityReady = true; // Mark entity as ready for retrieval.
        }
    }
};

CXMLReader::CXMLReader(std::shared_ptr<CDataSource> src) : DImplementation(new SImplementation(src)) {}

CXMLReader::~CXMLReader() = default;

// Returns true if the end of data has been reached and no entities are ready.
bool CXMLReader::End() const {
    return DImplementation->EndOfData && !DImplementation->IsEntityReady;
}

// Reads an XML entity, optionally skipping character data (CDATA).
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata) {
    // Return the current entity if one is ready and not skipping CDATA.
    if (DImplementation->IsEntityReady) {
        if (!(skipcdata && DImplementation->CurrentEntity.DType == SXMLEntity::EType::CharData)) {
            entity = std::move(DImplementation->CurrentEntity);
            DImplementation->IsEntityReady = false; // Reset entity readiness.
            return true;
        }
    }

    DImplementation->IsEntityReady = false; // Reset readiness from any previous state.
    std::vector<char> buffer(4096); // Buffer for reading data from the data source.
    while (!DImplementation->DataSource->End()) {
        int length = DImplementation->DataSource->Read(buffer, sizeof(buffer));
        if (length > 0) {
            // Parse the read data.
            if (XML_Parse(DImplementation->Parser, buffer.data(), length, XML_FALSE) == XML_STATUS_ERROR) {
                throw std::runtime_error("XML Parse error");
            }

            if (DImplementation->IsEntityReady) {
                return ReadEntity(entity, skipcdata);
            }
        } else {
            // Handle the end of data source.
            DImplementation->EndOfData = true;
            XML_Parse(DImplementation->Parser, "", 0, XML_TRUE); 
            if (DImplementation->IsEntityReady) {
                return ReadEntity(entity, skipcdata); 
            }
            break;
        }
    }

    return false; 
}
