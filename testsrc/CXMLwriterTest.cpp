#include "XMLWriter.h"
#include "StringDataSink.h"
#include <iostream>
#include <cassert>

void TestXMLWriter() {
    // 创建一个CStringDataSink实例
    auto stringSink = std::make_shared<CStringDataSink>();

    // 创建CXMLWriter实例，传入stringSink
    CXMLWriter writer(stringSink);

    // 创建并写入一些XML实体
    SXMLEntity startElement;
    startElement.DType = SXMLEntity::EType::StartElement;
    startElement.DNameData = "greeting";
    startElement.DAttributes.push_back({"language", "English"});
    writer.WriteEntity(startElement);

    SXMLEntity completeElement;
    completeElement.DType = SXMLEntity::EType::CompleteElement;
    completeElement.DNameData = "content";
    completeElement.DAttributes.push_back({"type", "text"});
    writer.WriteEntity(completeElement);

    SXMLEntity endElement;
    endElement.DType = SXMLEntity::EType::EndElement;
    endElement.DNameData = "greeting";
    writer.WriteEntity(endElement);

    // 刷新写入器以确保所有数据都被处理
    writer.Flush();

    // 检查最终字符串是否符合预期
    std::string expectedXML = R"(<greeting language="English"><content type="text"/></greeting>)";
    assert(stringSink->String() == expectedXML);

    // 如果没有抛出断言错误，则测试通过
    std::cout << "CXMLWriter Test Passed!" << std::endl;
}

int main() {
    TestXMLWriter();
    return 0;
}