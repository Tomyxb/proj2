# Project 2 README
Xinbang Yuan 920853308
Student 1: Partner1 Name (123456789)

Student 2: Partner2 Name (123456987)

## Project Status
CDSVReader.cpp completed
CDSVWriter.cpp ing
CDSVReaderTest.cpp completed
CDSVWriterTest.cpp ing
XMLReader.cpp on Zhongtian
XMLWriter.cpp on Zhongtian
XMLReaderTest.cpp ?
XMLWriterTest.cpp ?
Makefile ing
## Known Issues

## Code References

## Generative AI Use
I used ChatGPT for prompt 1, prompt2

### Prompt 1
Do I have to put sth in CDSVReader::~CDSVReader(){} for anything? 

### Response 1
Since all ptrs are smart ptrs which do not need to free spaces for, there is no need to write anything. 
//gpt
CDSVReader::~CDSVReader() {
// There is no need to manually release DImplementation, as unique_ptr will handle it automatically
}
### Chagnes 1
N/A

### Prompt 2
Which kinds of input is what the normal case should be?

### Response 2
A data type could be used to input the src 
class InputDataType : public CDataSource {
public:
    std::stringstream Stream;

    InputDataType(const std::string& data) {
        Stream.str(data);
    }

    virtual bool End() const override {
        return Stream.eof();
    }

    friend std::istream& getline(std::istream &is, InputDataType& source) {
        return std::getline(is, source.Stream);
    }
};


