#include "DSVReader.h"
#include <gtest/gtest.h>
#include <sstream>
#include <memory>


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


//all delimiter are tested for ','; '\t'; 'z';'' total 4 cases

//test for empty source
TEST(CDSVReader, empty1){
    auto emptyDataSource = std::make_shared<InputDataType>("");
    CDSVReader reader(emptyDataSource, ',');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.End());
    EXPECT_FALSE(reader.ReadRow(row));
}
TEST(CDSVReader, empty2){
    auto emptyDataSource = std::make_shared<InputDataType>("");
    CDSVReader reader(emptyDataSource, '\t');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.End());
    EXPECT_FALSE(reader.ReadRow(row));
}
TEST(CDSVReader, empty3){
    auto emptyDataSource = std::make_shared<InputDataType>("");
    CDSVReader reader(emptyDataSource, 'z');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.End());
    EXPECT_FALSE(reader.ReadRow(row));
}
TEST(CDSVReader, empty4){
    auto emptyDataSource=std::make_shared<InputDataType>("");
    CDSVReader reader(emptyDataSource, '');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.End());
    EXPECT_FALSE(reader.ReadRow(row));
}



//test for single line
TEST(CDSVReader, single1){
    auto dataSource = std::make_shared<InputDataType>("a,b,c");
    CDSVReader reader(dataSource, ',');
    std::vector<std::string> row;
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");
    EXPECT_EQ(row[1], "b");
    EXPECT_EQ(row[2], "c");
}
TEST(CDSVReader, single2){
    auto dataSource = std::make_shared<InputDataType>("a\tb\tc");
    CDSVReader reader(dataSource, '\t');
    std::vector<std::string> row;
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");
    EXPECT_EQ(row[1], "b");
    EXPECT_EQ(row[2], "c");
}
TEST(CDSVReader, single3){
    auto dataSource = std::make_shared<InputDataType>("azbzc");
    CDSVReader reader(dataSource, 'z');
    std::vector<std::string> row;
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");
    EXPECT_EQ(row[1], "b");
    EXPECT_EQ(row[2], "c");
}
TEST(CDSVReader, single4){
    auto dataSource = std::make_shared<InputDataType>("abc");
    CDSVReader reader(dataSource, '');
    std::vector<std::string> row;
    EXPECT_FALSE(reader.End());
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");
    EXPECT_EQ(row[1], "b");
    EXPECT_EQ(row[2], "c");
}



//2-3 lines
TEST(CDSVReader, lines1){
    auto dataSource = std::make_shared<InputDataType>("a,b,c\nd,e,f");
    CDSVReader reader(dataSource, ',');
    std::vector<std::string> row;

    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
  EXPECT_EQ(row[1], "e");
}
TEST(CDSVReader, lines2){
    auto dataSource = std::make_shared<InputDataType>("a\tb\tc\nd\te\tf");
    CDSVReader reader(dataSource, '\t');
    std::vector<std::string> row;

    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
  EXPECT_EQ(row[1], "e");
}
TEST(CDSVReader, lines3){
    auto dataSource = std::make_shared<InputDataType>("azbzc\ndzezf");
    CDSVReader reader(dataSource, 'z');
    std::vector<std::string> row;

    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[1], "e");
}
TEST(CDSVReader, lines4){
    auto dataSource = std::make_shared<InputDataType>("abc\ndef");
    CDSVReader reader(dataSource, '');
    std::vector<std::string> row;

    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[1], "e");
}



TEST(CDSVReader, lines5){
    auto dataSource = std::make_shared<InputDataType>("a,b,c\nd,e,f\ng,h,i,j");
    CDSVReader reader(dataSource, ',');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "d");
    EXPECT_EQ(row[1], "e");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 4);
    EXPECT_EQ(row[2], "i");
}TEST(CDSVReader, lines6){
    auto dataSource = std::make_shared<InputDataType>("a\tb\tc\nd\te\tf\ng\th\ti\tj");
    CDSVReader reader(dataSource, '\t');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "d");
    EXPECT_EQ(row[1], "e");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 4);
    EXPECT_EQ(row[2], "i");
}
TEST(CDSVReader, lines7){
    auto dataSource = std::make_shared<InputDataType>("azbzc\ndzezf\ngzhzizj");
    CDSVReader reader(dataSource, 'z');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "d");
    EXPECT_EQ(row[1], "e");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 4);
    EXPECT_EQ(row[2], "i");
}
TEST(CDSVReader, lines8){
    auto dataSource = std::make_shared<InputDataType>("abc\ndef\nghij");
    CDSVReader reader(dataSource, '');
    std::vector<std::string> row;
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "a");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 3);
    EXPECT_EQ(row[0], "d");
    EXPECT_EQ(row[1], "e");

    row.clear();
    EXPECT_TRUE(reader.ReadRow(row));
    EXPECT_EQ(row.size(), 4);
    EXPECT_EQ(row[2], "i");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
