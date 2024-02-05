#include "DSVWriter.h"
#include <gtest/gtest.h>
#include <memory>
#include <sstream>

//
class MockDataSink : public CDataSink {
public:
    std::string data;
    virtual bool Write(const std::string& str) override {
        data += str;
        return true;
    }
};

//basic
TEST(CDSVWriterTest, HandlesSimpleStringHelloWorld){
    auto sink=std::make_shared<MockDataSink>();
    CDSVWriter writer(sink, ','); 
    std::vector<std::string> row={"HelloWorld"};
    writer.WriteRow(row);
    std::string expected_output="HelloWorld\n";
    EXPECT_EQ(sink->data, expected_output);
}

//test for special symbols
//","
TEST(CDSVWriter, special){
    auto sink=std::make_shared<MockDataSink>();
    CDSVWriter writer(sink, ',');
    std::vector<std::string> row={
        "value1",
        "value with delimiter, inside",
        "value \"with\" quotes",
        "value with\nnewline"
    };
    writer.WriteRow(row);
    std::string expected_output="\"value1\",\"value with delimiter, inside\",\"value \"\"with\"\" quotes\",\"value with\nnewline\"\n";
    EXPECT_EQ(sink->data, expected_output);
}
//"\t"
TEST(CDSVWriterTest, HandlesCommaInStringWithTabDelimiter){
    auto sink=std::make_shared<MockDataSink>();
    CDSVWriter writer(sink, '\t');
    std::vector<std::string> row={"Hello, World"};
    writer.WriteRow(row);
    std::string expected_output="\"Hello, World\"\n";
    EXPECT_EQ(sink->data, expected_output);
}
//"z"
TEST(CDSVWriterTest, HandlesStringWithZDelimiter){
    auto sink=std::make_shared<MockDataSink>();
    CDSVWriter writer(sink, 'z');
    std::vector<std::string> row={"Hello, World"};
    writer.WriteRow(row);
    std::string expected_output="Hello, World\n";
    EXPECT_EQ(sink->data, expected_output);
}

int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
