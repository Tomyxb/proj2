# Project 2 README
Xinbang Yuan 920853308
Student 1: Partner1 Name (123456789)

Student 2: Partner2 Name (123456987)

## Project Status
CDSVReader.cpp completed
CDSVWriter.cpp N/A
CDSVReaderTest.cpp ing
CDSVWriterTest.cpp N/A
XMLReader.cpp on Zhongtian
XMLWriter.cpp on Zhongtian
XMLReaderTest.cpp ?
XMLWriterTest.cpp ?
Makefile ?
## Known Issues

## Code References

## Generative AI Use
I used ChatGPT for Prompt 1.

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



