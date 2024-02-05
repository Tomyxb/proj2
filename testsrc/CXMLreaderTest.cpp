#include "XMLReader.h"
#include <iostream>

void TestXMLReader() {
    // 定义一个简单的XML字符串
    std::string xmlData = R"(<root>
<child attribute="value">Text</child>
<empty />
</root>)";

    // 创建一个基于字符串的数据源
    auto dataSource = std::make_shared<CStringDataSource>(xmlData);

    // 创建XML读取器
    CXMLReader xmlReader(dataSource);

    // 循环读取并打印所有XML实体
    SXMLEntity entity;
    while (xmlReader.ReadEntity(entity)) {
        std::cout << "Entity Type: ";
        switch (entity.DType) {
            case SXMLEntity::EType::StartElement:
                std::cout << "Start Element, Name: " << entity.DNameData << std::endl;
                for (const auto& attr : entity.DAttributes) {
                    std::cout << "  Attribute: " << attr.first << " = " << attr.second << std::endl;
                }
                break;
            case SXMLEntity::EType::EndElement:
                std::cout << "End Element, Name: " << entity.DNameData << std::endl;
                break;
            case SXMLEntity::EType::CharData:
                std::cout << "Character Data: " << entity.DNameData << std::endl;
                break;
            default:
                break;
        }
    }

    if (xmlReader.End()) {
        std::cout << "Reached the end of the XML data." << std::endl;
    } else {
        std::cout << "XML reading was interrupted." << std::endl;
    }
}

int main() {
    TestXMLReader();
    return 0;
}