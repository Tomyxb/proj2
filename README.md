# Project 2 README
Xinbang Yuan 920853308
Student 1: Zhongtian Yu 919551080

## Project Status
done
## Known Issues

## Code References

## Generative AI Use
I used ChatGPT4.0 for prompt 1, prompt2, prompt3, prompt4

### Prompt 1
Do I have to put sth in CDSVReader::~CDSVReader(){} for anything? 

### Response 1
Since all ptrs are smart ptrs which do not need to free spaces for, there is no need to write anything. 
//gpt
CDSVReader::~CDSVReader() {
// There is no need to manually release DImplementation, as unique_ptr will handle it automatically
}


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

### Prompt 3
No extra situation dealing (eempty line, '"->,' ,testing "/" )

### Chagnes 1
add

### Prompt 4
same as prompt2 for CDSWriter
### Response 4
class MockDataSink : public CDataSink {
public:
    std::string data;

    virtual bool Write(const std::string& str) override {
        data += str;
        return true;
    }
};
